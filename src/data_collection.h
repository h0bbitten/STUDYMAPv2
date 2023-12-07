#define USERNAME_MAX_LENGTH 25
#define USERNAME_MIN_LENGTH 5
#define PASSWORD_MAX_LENGTH 25
#define PASSWORD_MIN_LENGTH 8
#define CPR_MAX_LENGTH 10
#define HASH_PASSWORD 100
#define MAX_LINE_LENGTH (USERNAME_MAX_LENGTH + PASSWORD_MAX_LENGTH + CPR_MAX_LENGTH)


//Defines a struct for user logins consisting of a username and a password
typedef struct
{
    char username[USERNAME_MAX_LENGTH];
    char password[PASSWORD_MAX_LENGTH];
    char cpr[CPR_MAX_LENGTH];
    char hash_password[HASH_PASSWORD];
}Logins;

extern Logins current_user;

void make_directory(const char *path);
void login();
Logins load_user();
char login_or_signup();