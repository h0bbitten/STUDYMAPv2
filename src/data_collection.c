
#include "data_collection.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


void login()
{
    //Creates a user of the type struct Logins and inputs the current user into it
    Logins current_user;
    current_user = load_user();
    //debug line
    printf("Username: %s, Password:  %s",current_user.username, current_user.password);
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

        //This opens the csv file Users in the "a" (append) mode
        Users = fopen("Users.csv", "a");
        //Error case: checks if the file is opened correctly
        if(Users == NULL)
        {
            printf("Error: File not found.\n");
            exit(1);
        }

        printf("Please enter a username that is no longer than %d characters\n>", USERNAME_MAX_LENGTH);
        scanf("%s", this_user.username);

        printf("Please enter a password that is no longer than %d characters\n>", PASSWORD_MAX_LENGTH);
        scanf("%s", this_user.password);

        fprintf(Users, "%s,%s", this_user.username, this_user.password);
        fclose(Users);

        return this_user;

    }
    if(y_n == 'L' || y_n == 'l')
    {
        //If the user picks login, we need to check if the username and password corresponds with a user in
        //our database(Users.csv).


        printf("Please enter your username\n>");
        scanf("%s", this_user.username);

        printf("Please enter your password\n>");
        scanf("%s", this_user.password);

        char line[MAX_LINE_LENGTH];
        bool found_username = false;
        bool found_password = false;

        //Reads csv file line by line
        while (fgets(line, MAX_LINE_LENGTH, Users) != NULL) {
            // Separates each element in line by ','
             char *username = strtok(line, ",");
            // Compare with 1st cell / indtil comma
            if (username != NULL && strcmp(username, this_user.username) == 0) {
                found_username = true;
                // Compare with 2nd cell / indtal andet comma
                char *password = strtok(NULL, ",");
                if (password != NULL && strcmp(password, this_user.password) == 0) {
                    found_password = true;
                    break;
                }
            }
        }
        fclose(Users);

        if (found_password == true && found_password == false){
            printf("Invalid password");
        }
        if (found_username && found_password) {
            return this_user;
        }
    }
    printf("Invalid username or password\n");
    exit(0);
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

