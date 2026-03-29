// Write a C program that accepts a file name as command line argument 
// Prints the file’s contents on console
// If the file does not exist, print some error on the screen

#include <stdio.h>

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("Error: No file name provided\n");
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");

    if (fp == NULL) {
        printf("Error: File does not exist or cannot be opened\n");
        return 1;
    }

    char ch;
    while ((ch = fgetc(fp)) != EOF) {
        printf("%c", ch);
    }

    fclose(fp);

    return 0;
}