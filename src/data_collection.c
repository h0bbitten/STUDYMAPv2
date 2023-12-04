#include "data_collection.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


Logins current_user;


//Set path for Users
char* users_path = {"Databases/Users.csv"};

void hash(char *str, unsigned int *result) {
    *result = 0;

    while (*str) {
        *result = (*result * 5) + *str++;
    }
}



void login()
{
    //Creates a user of the type struct Logins and inputs the current user into it
    current_user = load_user();
    unsigned int hashed_password;
    hash(current_user.password, &hashed_password);
    //debug line
    printf("Username: %s, Password:  %u, CPR: %s",current_user.username, hashed_password, current_user.cpr);
}

Logins load_user()
{
    //Creates a variable for a FILE, adds a YES/NO char for user interaction and adds a temporary user,
    //which is used for output
    FILE *Users;
    char y_n;
    Logins this_user;

    //Checks if a Users file exists and creates one if one doesn't exist
    Users = fopen("Users.csv", "r");
    if(Users == NULL)
    {
        Users = fopen("Users.csv", "w");
        fclose(Users);

        Users = fopen("Users.csv", "r");
    }
    y_n = login_or_signup();

    if(y_n == 'S' || y_n == 's')
    {
        //If the user sign_up, then we need to add a new user to our database(Users.csv)

        //This opens the csv file Users in the "r" (read) mode
        Users = fopen("Users.csv", "r");
        //Error case: checks if the file is opened correctly
        if(Users == NULL)
        {
            printf("Error: File not found.\n");
            exit(1);
        }

        int usernameExists;
        do {
            printf("Please enter a username that is no longer than %d characters\n>", USERNAME_MAX_LENGTH);
            scanf("%s", this_user.username);

            char line[MAX_LINE_LENGTH];
            usernameExists = 0;

            // Rewind the file to the beginning
            rewind(Users);

            while (fgets(line, sizeof(line), Users) != NULL) {
                // strtok modifies the input string, so create a copy to avoid side effects
                char tempLine[MAX_LINE_LENGTH];
                strcpy(tempLine, line);

                char *existingUsername = strtok(tempLine, ",");
                if (existingUsername != NULL && strcmp(existingUsername, this_user.username) == 0) {
                    usernameExists = 1;
                    printf("Username already exists. Please choose a different username.\n");
                    break;
                }
            }

        } while (usernameExists);

        fclose(Users);

        unsigned int hashed_password;

        printf("Please enter a password that is no longer than %d characters\n>", PASSWORD_MAX_LENGTH);
        scanf("%s", &hashed_password);

        fprintf(Users, "%s,%s,\n", this_user.username, this_user.password);

        bool correct_password = false;
        do {
            printf("Please enter your CPR-number\n>");
            scanf("%s", current_user.cpr);
            
            if (strlen(current_user.cpr) != CPR_MAX_LENGTH){
                printf("The entered CPR-number is not the correct length. Try again.\n");
            } else {
                correct_password = true;
            }
        } while(correct_password == false);

        //This opens the csv file Users in the "a" (append) mode
        Users = fopen("Users.csv", "a");
        //Error case: checks if the file is opened correctly
        if(Users == NULL)
        {
            printf("Error: File not found.\n");
            exit(1);
        }

        fprintf(Users, "%s,%u,%s\n", this_user.username, hashed_password, this_user.cpr);
        fclose(Users);

        return this_user;

    }
    if (y_n == 'L' || y_n == 'l') {
        bool found_username = false;
        bool found_password = false;
        unsigned int hashed_password;

        do {
            printf("Please enter your username\n>");
            scanf(" %s", this_user.username);

            printf("Please enter your password\n>");
            scanf(" %s", &hashed_password);

            char line[MAX_LINE_LENGTH];

            rewind(Users);

            while (fgets(line, MAX_LINE_LENGTH, Users) != NULL) {
                char *username = strtok(line, ",");
                if (username != NULL && strcmp(username, this_user.username) == 0) {
                    found_username = true;
                    char *password = strtok(NULL, ",");
                    if (password != NULL && atoi(password) == hashed_password) {
                        found_password = true;
                        fclose(Users);
                        break;
                    }
                }
            }

            if (found_username == false) {
                printf("Username not found. Please try again.\n");
            }
            else if (found_password == false) {
                printf("Invalid password. Please try again.\n");
            }
            else{
                return this_user;
            }
        } while (1); // Loop indtil der er et login der passer


    }
}
char login_or_signup()
{
    char input;
    do {
        printf("Login (L) or sign-up (S)? \n>");
        input = getchar();
        while(getchar() != '\n');
    } while (input != 'L' && input != 'l' && input != 'S' && input != 's');
    return input;
}