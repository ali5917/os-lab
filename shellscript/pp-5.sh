# Write a shell script that gives a menu to the user to do # the following stuff:
    # Create folder(s) with name(s) to be input by user.
    # List all folders in the present working directory.
    # Count all folders in the present working directory.

#!/bin/bash

createFolders() {
    read -p "Enter folder name(s) separated by space: " folderNames
    if [ -z "$folderNames" ]; then
        echo "Folder name(s) cannot be empty"
        return
    fi

    for folder in $folderNames; do
        if [ -d "$folder" ]; then
            echo "Folder '$folder' already exists."
        else
            mkdir "$folder"
            echo "Folder '$folder' created."
        fi
    done
}

listFolders() {
    echo "Folders in current directory:"
    for f in * ; do
        [ -d "$f" ] && echo "$f"
    done
}

countFolders() {
    count=0
    for dir in * ; do
        [ -d "$dir" ] && ((count++))
    done
    echo "Total number of folders: $count"
}

exitProgram() {
    echo "Exiting..."
    exit
}

while true; do
    echo "1. Create folder(s)"
    echo "2. List all folders"
    echo "3. Count all folders"
    echo "4. Exit"
    read -p "Enter your choice: " choice

    case $choice in
        1) createFolders ;;
        2) listFolders ;;
        3) countFolders ;;
        4) exitProgram ;;
        *) echo "Enter a valid choice (1-4)." ;;
    esac
done
