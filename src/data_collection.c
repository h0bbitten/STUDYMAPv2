//
// Created by Thomas on 13-11-2023.
//

#include "data_collection.h"
#include <stdio.h>
#include <stdlib.h>

//Defines a struct for user logins consisting of a username and a password
typedef struct
{
    char username[25];
    char password[25];
}Logins;
Logins load_user();

void login()
{
    //Creates a user of the type struct Logins and inputs the current user into it
    Logins current_user;
    current_user = load_user();
    //debug line
    printf("%s, %s",current_user.username, current_user.password);
}

Logins load_user()
{
    //Creates a variable for a FILE, adds a YES/NO char for user interaction and adds a temporary user,
    //which is used for output
    FILE *Users;
    char y_n;
    Logins this_user;

    //Lets user decide if they want to log into an existing user
    printf("Do you want to login to an existing user?\n Y or N\n>");
    scanf("%c", &y_n);

    if(y_n == 'Y')
    {
        //If the user picks YES, we need to check if the username and password corresponds with a user in
        //our database(Users.csv).
        /*
        TODO
         Code that checks if the username and password is in the system
         */

        //This opens the csv file Users in the "r" (read) mode
        Users = fopen("Users.csv", "r");
        //Error case: checks if the file is opened correctly
        if(Users == NULL)
        {
            printf("Error: File not found.\n");
            exit(1);
        }
    }else if(y_n == 'N')
    {
        //If the user picks NO, then we need to add a new user to our database(Users.csv)

        //This opens the csv file Users in the "a" (append) mode
        Users = fopen("Users.csv", "a");
        //Error case: checks if the file is opened correctly
        if(Users == NULL)
        {
            printf("Error: File not found.\n");
            exit(1);
        }

        printf("Please enter a username that is no longer than 25 characters\n>");
        scanf("%s", this_user.username);

        printf("Please enter a password that is no longer than 25 characters\n>");
        scanf("%s", this_user.password);

        fprintf(Users, "%s, %s\n", this_user.username, this_user.password);
        fclose(Users);
    }else printf("Invalid input");

    return this_user;
}

