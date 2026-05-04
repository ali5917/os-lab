// =========================================================
// Multi-Stage Student Attendance System using Shared Memory
// 
// 1. Attendance Producer: Writes 5 raw attendance records (1 for Present, 0 for Absent) to shared memory.
// 2. Attendance Analyzer: Reads the records, calculates total present/absent, and writes the summary back.
// 3. Attendance Reporter: Reads the summary, prints it to the console, and saves it to attendance_report.txt.
// =========================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define SHM_NAME "/attendance_shm"

// Structure to hold data in shared memory
struct AttendanceData {
    int records[5]; // 1 for Present, 0 for Absent
    int total_present;
    int total_absent;
    int status; // Used for synchronization: 0=Empty, 1=Produced, 2=Analyzed
};

int main() {
    int fd;
    struct AttendanceData *ptr;
    pid_t pid1, pid2;

    // Create shared memory object
    fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    // Set size
    ftruncate(fd, sizeof(struct AttendanceData));

    // Map memory
    ptr = (struct AttendanceData *)mmap(0, sizeof(struct AttendanceData), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }
    
    ptr->status = 0; // Initial state

    // ----------------- FIRST FORK -----------------
    pid1 = fork();

    if (pid1 > 0) {
        // ---------------- ATTENDANCE PRODUCER ----------------
        printf("Attendance Producer: Sending attendance records...\n");
        int raw_attendance[5] = {1, 0, 1, 1, 0}; // 1 = Present, 0 = Absent
        
        for (int i = 0; i < 5; i++) {
            ptr->records[i] = raw_attendance[i];
        }
        
        ptr->total_present = 0;
        ptr->total_absent = 0;
        ptr->status = 1; // Signal Analyzer that data is ready

        wait(NULL); // Wait for Analyzer to finish
        
        // Final Cleanup by Parent
        munmap(ptr, sizeof(struct AttendanceData));
        close(fd);
        shm_unlink(SHM_NAME);
    } 
    else {
        // ---------------- SECOND FORK -----------------
        pid2 = fork();

        if (pid2 > 0) {
            // ---------------- ATTENDANCE ANALYZER ----------------
            // Spinlock: Wait until Producer is done writing
            while(ptr->status != 1) { usleep(1000); }

            printf("Attendance Analyzer: Calculating attendance summary...\n");
            int present = 0, absent = 0;
            for (int i = 0; i < 5; i++) {
                if (ptr->records[i] == 1) present++;
                else absent++;
            }

            ptr->total_present = present;
            ptr->total_absent = absent;
            ptr->status = 2; // Signal Reporter that summary is ready

            wait(NULL); // Wait for Reporter to finish
            
            munmap(ptr, sizeof(struct AttendanceData));
            close(fd);
        } 
        else {
            // ---------------- ATTENDANCE REPORTER ----------------
            // Spinlock: Wait until Analyzer is done calculating
            while(ptr->status != 2) { usleep(1000); }

            printf("Attendance Reporter: Displaying and logging attendance summary...\n");
            
            // Display to Console
            printf("Total Students Present: %d\n", ptr->total_present);
            printf("Total Students Absent: %d\n", ptr->total_absent);
            printf("System: Attendance processed and logged successfully.\n");

            // Save to File
            FILE *file = fopen("attendance_report.txt", "w");
            if (file != NULL) {
                fprintf(file, "Total Students Present: %d\n", ptr->total_present);
                fprintf(file, "Total Students Absent: %d\n", ptr->total_absent);
                fclose(file);
            } else {
                perror("fopen");
            }
            
            munmap(ptr, sizeof(struct AttendanceData));
            close(fd);
        }
    }

    return 0;
}