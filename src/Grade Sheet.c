#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// Define a structure to hold user information
struct User {
    char cprNumber[11];  // Assuming CPR number is a string with 11 characters
    char gradeSheet[1000];  // Assuming a string to hold grade sheet
    char education[50];  // Assuming a string to hold education information
};

// Function to find a user by CPR number
bool findUserByCPR(const char *cprNumber, const struct User users[], int numUsers, struct User *resultUser) {
    for (int i = 0; i < numUsers; ++i) {
        if (strcmp(users[i].cprNumber, cprNumber) == 0) {
            *resultUser = users[i];
            return true;  // User found
        }
    }
    return false;  // User not found
}

int main() {
    // Create sample users
    struct User users[3] = {
            {"1111111111", "Dansk A, mdt.:07, Dansk A, skr.: 07, Engelsk A, mdt.: 07, Engelsk A, skr.:07, Mat A, mdt.: 07", "STX"},
            {"2222222222", "Dansk A, mdt.:07, Dansk A, skr.: 07, Engelsk A, mdt.: 07, Engelsk A, skr.:07, Mat A, mdt.: 07", "HTX"},
            {"3333333333", "Dansk A, mdt.:07, Dansk A, skr.: 07, Engelsk A, mdt.: 07, Engelsk A, skr.:07, Mat A, mdt.: 07",}
    };

    // Ask the user to input the CPR number to search for
    char searchCPR[11];
    printf("Enter CPR number to search for: ");
    scanf("%s", searchCPR);

    struct User foundUser;

    // Search for a user by CPR number
    if (findUserByCPR(searchCPR, users, 3, &foundUser)) {
        printf("User found:\n");
        printf("CPR Number: %s\n", foundUser.cprNumber);
        printf("Grade Sheet: %s\n", foundUser.gradeSheet);

        if (strlen(foundUser.education) > 0) {
            printf("Education: %s\n", foundUser.education);
        } else {
            // If education information is not available, ask the user to input it - dette indsætter ikke informationen i CSV filen - midlertidig løsning
            printf("Education not available. Enter education: ");
            scanf("%s", foundUser.education);
            printf("Education: %s\n", foundUser.education);
        }
    } else {
        printf("User with CPR Number %s not found\n", searchCPR);
    }

    return 0;
}
