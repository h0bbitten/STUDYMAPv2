#define USERNAME_MAX_LENGTH 25
#define PASSWORD_MAX_LENGTH 25
#define CPR_MAX_LENGTH 11
#define MAX_LINE_LENGTH (USERNAME_MAX_LENGTH + PASSWORD_MAX_LENGTH + CPR_MAX_LENGTH)
//Defines a struct for user logins consisting of a username and a password
typedef struct
{
    char username[USERNAME_MAX_LENGTH];
    char password[PASSWORD_MAX_LENGTH];
    char cpr[CPR_MAX_LENGTH];
}Logins;

extern Logins current_user;

void login();
Logins load_user();
char login_or_signup();