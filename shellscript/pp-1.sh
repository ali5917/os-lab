# The script should display a menu with the following options:
    # Add a new task (user inputs task name and priority: High, Medium, or Low).
    # View all tasks.
    # Display tasks sorted by priority (High → Medium → Low).
    # Mark a task as completed (remove from the list after confirmation).
    # Exit the program.
# The script should validate inputs, handle errors (such as empty task names), and store tasks in a text file. 
# The menu should run in a loop until the user chooses to exit

#!/bin/bash

add_task () {
    name=""
    while [ -z "$name" ]; do
        read -p "Task name: " name
        if [ -z "$name" ]; then            
            echo "Task name cannot be empty"
        fi 
    done    

    priority=""
    while [ -z "$priority" ]; do
        read -p "Task priority (High/Medium/Low): " priority
        if [[ "$priority" != "High" && "$priority" != "Medium" && "$priority" != "Low" ]]; then            
            echo "Task priority can only be (High/Medium/Low)"
            priority=""
        fi 
    done

    echo "$name, Priority: $priority" >> tasks.txt    
}

view_tasks () {
    while read -r line; do
        echo "$line"
    done < tasks.txt
}

sort_tasks () {
    echo "High Priority"
    grep "Priority: High" tasks.txt
    
    echo "Medium Priority"
    grep "Priority: Medium" tasks.txt
    
    echo "Low Priority"
    grep "Priority: Low" tasks.txt
}

mark_task () {
    read -p "Enter task name to be removed: " name
    read -p "Are you sure? (y/n): " ans

    if [ "$ans" = "y" ]; then 
    grep -vF "$name" tasks.txt > temp.txt
        mv temp.txt tasks.txt 
    fi

    echo "Task removed!"
}

exit_loop () {
    echo "Exiting..."
    exit
}

while true; do
    echo "Choose any option!"
    echo "  1. Add a new task"
    echo "  2. View all tasks"
    echo "  3. Sort tasks (Priority: High -> Low)"
    echo "  4. Mark task as completed"
    echo "  5. Exit"

    read -p "Enter choice: " choice

    case $choice in
        1) add_task ;; 
        2) view_tasks ;;
        3) sort_tasks ;;
        4) mark_task ;;
        5) exit_loop ;;
        *) echo "Invalid Choice. Choose (1-5)" ;;
    esac

done