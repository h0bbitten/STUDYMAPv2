#include "data_collection.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#ifdef _WIN32
    #include <direct.h>
    #include <errno.h>
    #include <windows.h>
#else
    #include <sys/stat.h>
    #include <errno.h>
#endif


Logins current_user;

void make_directory(const char *path) {
#ifdef _WIN32
    if (_mkdir(path) != 0) {
        if (errno != EEXIST && errno != ERROR_ALREADY_EXISTS) {
            fprintf(stderr, "Failed to create directory: %s.\n", path);
        }
    }
#else
    if (mkdir(path, 0777) != 0) {
        if (errno != EEXIST) {
            fprintf(stderr, "Failed to create directory: %s.\n", path);
        }
    }
#endif
}

//Set path for Users
char* users_path = {"Databases/Users/Users.csv"};

void hash(char *str, unsigned int *result) {
    *result = 0;

    while (*str) {
        *result = (*result * 5) + *str++;
    }
}

void login() {
    //Creates a user of the type struct Logins and inputs the current user into it
    current_user = load_user();
    unsigned int hashed_password;

    //debug line
    printf("Username: %s, Password:  %u, CPR: %s",current_user.username, hashed_password, current_user.cpr);
    
}


Logins load_user() {

    //Create directory for databases
    make_directory("Databases");


    //Create directory for users
    make_directory("Databases/Users");

        //Creates a variable for a FILE, adds a YES/NO char for user interaction and adds a temporary user,
        //which is used for output
        FILE *Users;
        char login_signup;
        Logins this_user;

        //Checks if a Users file exists and creates one if one doesn't exist
        Users = fopen(users_path, "r");
        if (Users == NULL) {
            Users = fopen(users_path, "w");
            fclose(Users);

            Users = fopen(users_path, "r");
        }
        login_signup = login_or_signup();

        if (login_signup == 'S' || login_signup == 's') {
            //If the user sign_up, then we need to add a new user to our database(Users.csv)

            //This opens the csv file Users in the "r" (read) mode
            Users = fopen(users_path, "r");
            //Error case: checks if the file is opened correctly
            if (Users == NULL) {
                printf("Error: File not found.\n");
                exit(1);
            }

            int usernameExists;

            do {
                bool correct_username = false;
                do {
                    printf("Please enter a username (Min. %d characters & Max. %d characters)\n>", USERNAME_MIN_LENGTH,
                           USERNAME_MAX_LENGTH);
                    scanf("%s", this_user.username);

                    if (strlen(this_user.username) < USERNAME_MIN_LENGTH) {
                        printf("The entered username must be at least 5 characters. Try again.\n");
                    } else {
                        correct_username = true;
                    }
                } while (correct_username == false);

                char line[MAX_LINE_LENGTH];
                usernameExists = 0;

                rewind(Users);

                while (fgets(line, sizeof(line), Users) != NULL) {
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

            bool correct_password = false;
            do {
                printf("Please enter a password (Min. %d characters & Max. %d characters)\n>", PASSWORD_MIN_LENGTH,
                       PASSWORD_MAX_LENGTH);
                scanf("%s", this_user.password);

                if (strlen(this_user.password) < PASSWORD_MIN_LENGTH) {
                    printf("The entered password must be at least 8 characters. Try again.\n");
                } else {
                    correct_password = true;
                }
            } while (correct_password == false);


            bool correct_cpr = false;
            do {
                printf("Please enter your CPR-number.\n>");
                scanf("%s", this_user.cpr);

                if (strlen(this_user.cpr) != CPR_MAX_LENGTH) {
                    printf("The entered CPR-number is not the correct length. Try again.\n");
                } else {
                    correct_cpr = true;
                }
            } while (correct_cpr == false);

            printf("Enter MatA or MatB.\n>");
            scanf("%s", this_user.requirements);

            //This opens the csv file Users in the "a" (append) mode
            Users = fopen(users_path, "a");
            //Error case: checks if the file is opened correctly
            if (Users == NULL) {
                printf("Error: File not found.\n");
                exit(1);
            }

            hash(this_user.password, &hashed_password);

            fprintf(Users, "%s,%u,%s,%s\n", this_user.username, hashed_password, this_user.cpr, this_user.requirements);
            fclose(Users);

            return this_user;

        }

        if (login_signup == 'L' || login_signup == 'l') {
            bool found_username = false;
            bool found_password = false;
            unsigned int hashed_password;

            do {
                printf("Please enter your username\n>");
                scanf(" %s", this_user.username);

                printf("Please enter your password\n>");
                scanf(" %s", this_user.password);
                hash(this_user.password, &hashed_password);
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
                } else if (found_password == false) {
                    printf("Invalid password. Please try again.\n");
                } else {
                    return this_user;
                }
            } while (1); // Loop indtil der er et login der passer
        }
    }

char login_or_signup() {
    char input;
    do {
        printf("Login (L) or sign-up (S)? \n>");
        input = getchar();
        while(getchar() != '\n');
    } while (input != 'L' && input != 'l' && input != 'S' && input != 's');
    return input;
}