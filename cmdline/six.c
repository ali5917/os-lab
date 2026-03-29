// Write a program that takes a string input from the user
// Display a menu using a switch statement 
//      1. Find length of string
//      2. Convert to uppercase
//      3. Convert to lowercase
//      4. Reverse the string
//      5. Check if palindrome

#include <stdio.h>
#include <string.h>
#include <ctype.h>

int findLength(char str[]) {
    return strlen(str);
}

void toUpperCase(char str[]) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = toupper(str[i]);
    }
    printf("Uppercase: %s\n", str);
}

void toLowerCase(char str[]) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower(str[i]);
    }
    printf("Lowercase: %s\n", str);
}

void reverseString(char str[]) {
    int len = strlen(str);
    char temp;

    for (int i = 0; i < len / 2; i++) {
        temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }

    printf("Reversed: %s\n", str);
}

void checkPalindrome(char str[]) {
    int len = strlen(str);
    int isPalindrome = 1;

    for (int i = 0; i < len / 2; i++) {
        if (str[i] != str[len - i - 1]) {
            isPalindrome = 0;
            break;
        }
    }

    if (isPalindrome)
        printf("The string is a palindrome\n");
    else
        printf("The string is NOT a palindrome\n");
}

int main() {
    char str[100];
    int choice;

    printf("Enter a string: ");
    fgets(str, sizeof(str), stdin);

    // Remove newline from fgets
    str[strcspn(str, "\n")] = '\0';

    printf("\nMenu:\n");
    printf("1. Find length\n");
    printf("2. Convert to uppercase\n");
    printf("3. Convert to lowercase\n");
    printf("4. Reverse string\n");
    printf("5. Check palindrome\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            printf("Length: %d\n", findLength(str));
            break;

        case 2:
            toUpperCase(str);
            break;

        case 3:
            toLowerCase(str);
            break;

        case 4:
            reverseString(str);
            break;

        case 5:
            checkPalindrome(str);
            break;

        default:
            printf("Invalid choice\n");
    }

    return 0;
}