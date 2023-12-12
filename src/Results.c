#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Results.h"
#include "data_collection.h"
#include "load_profile.h"
#include "KNN.h"
typedef struct {
    char* name;
    double value;

} results;

typedef struct {
    char* name;
    char* description;

} educations;

int file_count = 8;


void read_results(char* file_path, results result[8]);

void read_edu_data(char* file_path, educations education[8]);

void filter_results(results result[8], educations education[8]);

void print_results(results result[8], educations education[8], int num_to_print);


void Display_results() {
    char* edu_data = "Databases/Uni.csv";

    results result[file_count];

    educations education[file_count];

    read_results(result_path, result);

    read_edu_data(edu_data, education);

    filter_results(result, education);

    int k = 8;

    printf("These are the top %d recommended results for %s:\n\n", k, current_user.username);
    print_results(result, education, k);

    // Free allocated memory
    for (int i = 0; i < file_count; i++) {
        free(result[i].name);
        free(education[i].name);
        free(education[i].description);
    }
}

void read_results(char* file_path, results result[8]) {
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        fprintf(stderr, "Failed to open %s.\n", file_path);
    }

    char line[MAX_LEN];
    int count = 0;

    while (fgets(line, sizeof(line), file) && count < 8) {
        char *token = strtok(line, DELIMITER);

        if (token != NULL) {
            result[count].name = malloc(strlen(token) + 1);
            strcpy(result[count].name, token);

            token = strtok(NULL, DELIMITER);
            if (token != NULL) {
                result[count].value = atof(token);
                count++;
            }
        }
    }

    fclose(file);

}
void read_edu_data(char* file_path, educations education[8]) {
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        fprintf(stderr, "Failed to open %s.\n", file_path);
    }

    char line[MAX_LEN];
    int count = 0;

    while (fgets(line, sizeof(line), file) && count < 8) {
        char *token = strtok(line, DELIMITER);

        if (token != NULL) {
            education[count].name = malloc(strlen(token) + 1);
            strcpy(education[count].name, token);

            token = strtok(NULL, DELIMITER);
            if (token != NULL) {
                education[count].description = malloc(strlen(token) + 1);
                strcpy(education[count].description, token);
                count++;
            }
        }
    }

    fclose(file);

}
void print_results(results result[8], educations education[8], int num_to_print) {
    double ref_distance = 27.0;

    printf("Recommended results for %s:\n\n", current_user.username);

    for (int i = 0; i < num_to_print; i++) {
        if (result[i].value != -1.0) {  // Check if the value is not filtered
            for (int j = 0; j < file_count; j++) {
                if (strcmp(result[i].name, education[j].name) == 0) {
                    double percentage = ((1 - (result[i].value / ref_distance)) * 100);
                    printf("%s %.2f%% match:\n%s\n", result[i].name, percentage, education[j].description);
                    break;
                }
            }
        }
    }
}



void read_user_data(char* file_path) {
    FILE* file = fopen(file_path, "r");
    if (file == NULL) {
        fprintf(stderr, "Failed to open %s.\n", file_path);
        exit(EXIT_FAILURE);
    }

    char line[MAX_LEN];
    if (fgets(line, sizeof(line), file)) {
        char* token = strtok(line, DELIMITER);
        strcpy(current_user.username, token);
        printf("Username: %s\n", current_user.username); // Debug statement

        token = strtok(NULL, DELIMITER);
        strcpy(current_user.password, token);
        printf("Password: %s\n", current_user.password); // Debug statement

        token = strtok(NULL, DELIMITER);
        strcpy(current_user.cpr, token);
        printf("CPR: %s\n", current_user.cpr); // Debug statement

        token = strtok(NULL, DELIMITER);
        strcpy(current_user.requirements, token);
        printf("Requirements: %s\n", current_user.requirements); // Debug statement
    } else {
        printf("Failed to read user data.\n");
    }

    fclose(file);
}



void filter_results(results result[8], educations education[8]) {
    // Attempt to open the Users.csv file to find the current user's grade
    FILE* users_file = fopen("Databases/Users/Users.csv", "r");
    if (users_file == NULL) {
        fprintf(stderr, "Failed to open Users.csv.\n");
        return;
    }

    char line[256];
    char user_grade[10] = {0}; // Buffer to hold the user's grade
    bool grade_found = false;

    // Read through the Users.csv file to find the current user's grade
    while (fgets(line, sizeof(line), users_file)) {
        // Tokenizing the line by commas
        char* token = strtok(line, ",");
        if (strcmp(token, current_user.username) == 0) {
            strtok(NULL, ","); // Skip password
            strtok(NULL, ","); // Skip CPR
            token = strtok(NULL, ",\n"); // Grade should be here
            if (token) {
                strncpy(user_grade, token, sizeof(user_grade) - 1);
                user_grade[sizeof(user_grade) - 1] = '\0'; // Ensure null-termination
                grade_found = true;
                break;
            }
        }
    }
    fclose(users_file);

    // If the user's grade wasn't found, we cannot filter based on grade
    if (!grade_found) {
        fprintf(stderr, "User's grade not found. Cannot filter results.\n");
        return;
    }

    // Go through each education and filter based on the user's grade
    for (int i = 0; i < file_count; i++) {
        char edu_file_path[PATH_MAX];
        snprintf(edu_file_path, PATH_MAX, "Databases/Edu_data/%s.csv", education[i].name);

        FILE* edu_file = fopen(edu_file_path, "r");
        if (!edu_file) {
            fprintf(stderr, "Error opening file: %s\n", edu_file_path);
            continue;
        }

        bool requirement_met = true;
        while (fgets(line, sizeof(line), edu_file)) {
            char* requirement = strrchr(line, ',') + 1; // Requirement is after the last comma
            if (requirement && strncmp(requirement, "MatA", 4) == 0 && strncmp(user_grade, "MatB", 4) == 0) {
                // If the requirement is MatA and user has MatB, they don't meet the requirement
                requirement_met = false;
                break;
            }
        }
        fclose(edu_file);

        if (!requirement_met) {
            result[i].value = -1.0; // Filter out this education
        }
    }
}
