# The script should display menu to: 
# View all running processes 
# Search for a specific process by name
# Kill a process by entering its Process ID (PID)
# Check system resource usage, including CPU and memory utilization. 
# Before performing any operation, the script should validate user input
# If an attempt is made to kill a non-existent process, an appropriate error message should be displayed. 
# The script should run continuously in a loop until the user chooses to exit

#!/bin/bash

view_all () {
    ps aux | less
}

search_process () {
    read -p "Enter process name to search: " pname
    if [[ -z "$pname" ]]; then
        echo "Process name cannot be empty."
    else
        ps aux | grep "$pname"
    fi
}

kill_process () {
    read -p "Enter process ID: " id
    if [ -z "$id" ]; then
        echo "Process ID can not be empty"
    elif ! ps -p "$id" > /dev/null; then
        echo "Process ID doesn't exit"
    else
        kill "$id"
        echo "PID $id killed"
    fi
}

check_resource () {
    echo "System Resources Usage"
    top | head -n 10
}
  
exit_loop () {
    echo "Exiting..."
    exit
}

while true; do
    echo "Choose any option!"
    echo "  1. View all processes"
    echo "  2. Search process by name"
    echo "  3. Kill process"
    echo "  4. Check system resource usage"
    echo "  5. Exit"

    read -p "Enter choice: " choice

    case $choice in
        1) view_all ;; 
        2) search_process ;;
        3) kill_process ;;
        4) check_resource ;;
        5) exit_loop ;;
        *) echo "Invalid Choice. Choose (1-5)" ;;
    esac

done