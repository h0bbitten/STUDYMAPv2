#include "data_collection.h"
#include "stdio.h"
#include "string.h"

#define MAX_LINE_SIZE 112

typedef struct {
    char username[50];
    char password[50];
    char cprNumber[11];
} UserData;
//Opens Users.csv in read-mode
int searchUserByCPR(const char *cprNumber) {
    FILE *file = fopen("databases/Users/Users.csv", "r");
    if (!file) {
        perror("Error opening file");
        return -1; // File not found or unable to open
    }

    char line[MAX_LINE_SIZE];
    while (fgets(line, sizeof(line), file) != NULL) {
        UserData user;
        sscanf(line, "%49[^,],%49[^,],%11s", user.username, user.password, user.cprNumber);

        // Compare the CPR number
        if (strcmp(user.cprNumber, cprNumber) == 0) {
            printf("User Information:\n");
            printf("Username: %s\n", user.username);
            printf("CPR Number: %s\n", user.cprNumber);
            fclose(file);
            return 1; // Found the user
        }
    }

    fclose(file);
    return 0; // CPR number not found
}
//search function
int SearchCPR() {
    char inputCPR[12];

    printf("Enter CPR number to search: ");
    scanf("%10s", inputCPR);

    int result = searchUserByCPR(inputCPR);
//Error messages in case of CPR not being in the file or the file failing to open.
    if (result == 0) {
        printf("User with CPR number %s not found.\n", inputCPR);
    } else if (result == -1) {
        printf("Error occurred while opening the file.\n");
    }

    return 0;
}

int main() {
    return SearchCPR();
}
