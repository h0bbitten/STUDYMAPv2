
//Defines a struct for user logins consisting of a username and a password
typedef struct
{
    char username[25];
    char password[25];
}Logins;



void login();
Logins load_user();
char yes_or_no_input();