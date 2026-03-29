# Write a shell script that defines a function called search_word, takes two arguments: a word and a filename.
# Inside the function, search for the word in the given file
# Display all matching lines along with their line numbers.
# If the word is not found, the function should print: "Word not found in the file."
# The script should call this function after reading input from the user.

#! /bin/bash

search_word () {
    local word="$1"
    local filename="$2"

    if [ ! -f "$filename" ]; then
        echo "File '$filename' does not exist."
        return
    fi

    result=$(grep -n "$word" "$filename")
    if [ -z "$result" ]; then 
        echo "Word not found in the file."
    else
        echo "Match(es) found: "
        echo "$result"
    fi
}

read -p "Enter word to find: " word 
read -p "Enter file name to be searched: " filename 

search_word "$word" "$filename"