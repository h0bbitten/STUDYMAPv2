//
// Created by Thomas on 13-11-2023.
//

#include "data_collection.h"
#include <stdio.h>
#include <stdlib.h>


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

    y_n = yes_or_no_input();

    //Checks if a Users file exists and creates one if one doesnt exist
    Users = fopen("Users.csv", "r");
    if(Users == NULL)
    {
        Users = fopen("Users.csv", "w");
    }


    if(y_n == 'Y' || y_n == 'y')
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

        //Besked til Jonas:
        //Tuborg klammer hører til på næste linje
    }else if(y_n == 'N' || y_n == 'n')
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

char yes_or_no_input()
{
    char input;
    do {
        printf("Do you want to login to an existing user? y/n >");
        input = getchar();
        while(getchar() != '\n');
    } while (input != 'y' && input != 'n' && input != 'Y' && input != 'N');
    return input;
}

