# Write a shell script that gives a menu to the user to do the following stuff: 
# Create text file(s) with name(s) to be input by user.
# Open text file(s) to edit. Name(s) of the file(s) to be input by user.
# Change permissions of a file (name input by user) to all.

#!/bin/bash

createFiles () {
    read -p "Enter name(s) of text file(s): " fileNames
    if [ -z "$fileNames" ]; then
        echo "Filename(s) cannot be empty"
    fi

    for f in $fileNames; do
        if [ -f "$f" ]; then
            echo "File already exists"
        else 
            touch "$f"
        fi
    done
}

openFiles () {
    read -p "Enter name(s) of text file(s): " fileNames
    if [ -z "$fileNames" ]; then
        echo "Filename(s) cannot be empty"
    fi

    for f in $fileNames; do
        if [ ! -f "$f" ]; then
            echo "File doesn't exists"
        else 
            nano "$f"
        fi
    done
}

changePerm () {
    read -p "Enter name of text file: " fileName
    if [ -z "$fileName" ]; then
        echo "Filename cannot be empty"
    elif [ ! -f "$fileName" ]; then
        echo "File doesn't exists"
    else 
        chmod 777 "$fileName"
    fi
}

exitProgram () {
    echo "Exiting..."
    exit
}

while true; do
    echo "1. Create text files"
    echo "2. Open text files"
    echo "3. Change file's permissions"
    echo "4. Exit"
    read -p "Enter choice: " choice

    case $choice in 
        1) createFiles ;;
        2) openFiles ;;
        3) changePerm ;;
        4) exitProgram ;;
        *) echo -e "Enter valid choice\n"
    esac
done