# Allow the user to input the following details for the company using command line arguments (separated by space): 
# Company name, and Names and Salaries of 3 employees. 
# Once the data is entered, display a menu with the following options:
    # Calculate the average salary of the company.
    # Display the employee with the highest salary.
    # Sort employees by salary and display the sorted list along with employee name
    # Save employee records to a text file and view the text file.
# Exit the program.

#!/bin/bash

company="$1"
emp1="$2"
emp2="$3"
emp3="$4"

read n1 s1 <<< "$emp1"
read n2 s2 <<< "$emp2"
read n3 s3 <<< "$emp3"

avgSalary () {
    avg=$((s1+s2+s3))
    avg=$((avg/3))
    echo "Average Salary: $avg"
}

highestSalary () {
    highest=$s1
    if [ "$s2" -gt "$highest" ]; then
        highest=$s2
    fi

    if [ "$s3" -gt "$highest" ]; then
        highest=$s3
    fi

    echo "Highest salary is $highest given to"
    if [ "$s1" -eq "$highest" ]; then
            echo "$n1"
    fi

    if [ "$s2" -eq "$highest" ]; then
            echo "$n2"
    fi

    if [ "$s3" -eq "$highest" ]; then
            echo "$n3"
    fi
}

sortSalary () {
    echo "Sorted salaries (High to Low)"
    echo -e "$s1 $n1\n$s2 $n2\n$s3 $n3" | sort -nr    
}

saveView () {
    echo -e "$s1 $n1\n$s2 $n2\n$s3 $n3" >> salary.txt
    cat salary.txt 
}

exitProgram () {
    echo "Exiting..."
    exit
}

while true; do
    echo "Company: $company"
    echo "  1. Average salary"
    echo "  2. Display highest salary"
    echo "  3. Sort salaries (descending)"
    echo "  4. Save to file and View"
    echo "  5. Exit"
    read -p "Enter choice: " choice

    case $choice in 
        1) avgSalary ;;
        2) highestSalary ;;
        3) sortSalary ;;
        4) saveView ;;
        5) exitProgram ;;
        *) echo -e "Enter valid choice\n"
    esac
done