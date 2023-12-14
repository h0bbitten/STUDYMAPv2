#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "Results.h"
#include "Registration.h"
#include "Main_menu.h"
#include "KNN.h"
#include "sys/stat.h"



void incrementFileCount() {
    numFiles++;
}


int countFilesInFolder(const char *folderPath) {
    numFiles = 0;
    DIR *dir;
    struct dirent *entry;
    struct stat fileStat;

    dir = opendir(folderPath);
    if (dir == NULL) {
        perror("Unable to open directory");
        return -1;
    }

    while ((entry = readdir(dir)) != NULL && numFiles < MAX_FILES) {
        char fullPath[1024]; // Assuming a maximum path length of 1024
        snprintf(fullPath, sizeof(fullPath), "%s/%s", folderPath, entry->d_name);

        if (stat(fullPath, &fileStat) == 0) {
            if (S_ISREG(fileStat.st_mode)) {
                incrementFileCount();
            }
        }
    }

    closedir(dir);
    return numFiles; // Return the number of files found
}


void Display_results() {
    char* edu_data = "Databases/Uni.csv";

    results result[numFiles];
    educations education[numFiles];
    results filteredResults[numFiles];
    int numFilteredResults = 0;

    read_results(result_path, result);
    read_edu_data(edu_data, education);

    filter_results(result, education, filteredResults, &numFilteredResults);

    int k = 3; // Number of top results to display, adjust as needed
    int numToDisplay = numFilteredResults < k ? numFilteredResults : k;

    printf("These are the top %d recommended results for %s:\n\n", numToDisplay, current_user.username);

    // Use filteredResults for printing
    print_results(filteredResults, education, numToDisplay);

    // Free allocated memory
    for (int i = 0; i < numFiles; i++) {
        //free(result[i].name);
        free(education[i].name);
        free(education[i].link);
        free(education[i].description);
    }
}




void read_results(char* file_path, results result[14]) {
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        fprintf(stderr, "Failed to open %s.\n", file_path);
    }

    char line[MAX_LEN];
    int count = 0;

    while (fgets(line, sizeof(line), file) && count < 14) {
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

void read_edu_data(char* file_path, educations education[14]) {
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        fprintf(stderr, "Failed to open %s.\n", file_path);
    }

    char line[MAX_LEN];
    int count = 0;

    while (fgets(line, sizeof(line), file) && count < 14) {
        char *token = strtok(line, DELIMITER);

        if (token != NULL) {
            education[count].name = malloc(strlen(token) + 1);
            strcpy(education[count].name, token);

            token = strtok(NULL, DELIMITER);
            if (token != NULL) {
                education[count].link = malloc(strlen(token) + 1);
                strcpy(education[count].link, token);

                // Start concatenating tokens from the third element
                token = strtok(NULL, DELIMITER);
                if (token != NULL) {
                    education[count].description = malloc(strlen(token) + 1);
                    strcpy(education[count].description, token);

                    // Concatenate the rest of the line until newline character
                    while ((token = strtok(NULL, "\n")) != NULL) {
                        education[count].description = realloc(education[count].description, strlen(education[count].description) + strlen(token) + 2);
                        strcat(education[count].description, " ");
                        strcat(education[count].description, token);
                    }

                    // Remove the last character in education.description
                    if (strlen(education[count].description) > 0) {
                        education[count].description[strlen(education[count].description) - 1] = '\0';
                    }

                    count++;
                }
            }
        }
    }
    fclose(file);
}

void print_results(results result[numFiles], educations education[numFiles], int num_to_print) {
    double ref_distance = 27.0;

    for (int i = 0; i < num_to_print; i++) {
        for (int j = 0; j < numFiles; j++) {
            if (strcmp(result[i].name, education[j].name) == 0 && result[i].value != -1.0) {  // Check if result is not filtered out
                double percentage = ((1 - (result[i].value / ref_distance)) * 100);

                printf("%s (%.2f%% match):\n", result[i].name, percentage);

                // Print education.description with line breaks at spaces
                int line_length = 0;
                char *token = strtok(education[j].description, " ");
                while (token != NULL) {
                    if (line_length + strlen(token) + 1 > LINE_WIDTH) {
                        printf("\n");
                        line_length = 0;
                    }
                    printf("%s ", token);
                    line_length += strlen(token) + 1;
                    token = strtok(NULL, " ");
                }

                printf("\nTo see more, check out: %s\n\n", education[j].link);
            }
        }
    }
}


bool isGradeCompatible(const char* requiredGrade, const char* userGrade) {
    // Check if userGrade is 'MatA' (highest compatibility)
    if (strcmp(userGrade, "MatA") == 0) {
        return true; // If yes, return true as 'MatA' is compatible with any grade
    }
        // Check if userGrade is 'MatB'
    else if (strcmp(userGrade, "MatB") == 0) {
        // If yes, check if requiredGrade is also 'MatB' as 'MatB' is only compatible with itself
        return strcmp(requiredGrade, "MatB") == 0;
    }
    // If userGrade is neither 'MatA' nor 'MatB', return false indicating incompatibility
    return false;
}

void filter_results(results result[numFiles], educations education[numFiles], results filteredResults[], int *numFilteredResults) {
    char user_grade[10]; // Array to store the user's grade
    bool grade_found = false; // Flag to track if user's grade is found
    char line[256]; // Buffer to read lines from file
    char* token; // Token for parsing strings

    // Open the file containing user data
    FILE* users_file = fopen("Databases/Users/Users.csv", "r");
    if (users_file == NULL) {
        fprintf(stderr, "Failed to open Users.csv.\n"); // Error handling if file opening fails
        return;
    }

    printf("Opened Users.csv successfully.\n"); // Indicate successful file opening

    // Loop to find the user's grade in the file
    while (fgets(line, sizeof(line), users_file)) {
        token = strtok(line, ","); // Parse username
        if (strcmp(token, current_user.username) == 0) {
            strtok(NULL, ",");  // Skip password
            strtok(NULL, ",");  // Skip CPR
            token = strtok(NULL, ",\n");  // Read grade
            if (token) {
                strncpy(user_grade, token, sizeof(user_grade) - 1); // Copy the grade to user_grade
                user_grade[sizeof(user_grade) - 1] = '\0'; // Ensure null-termination
                grade_found = true;
                break; // Break loop as grade is found
            }
        }
    }

    fclose(users_file); // Close the file

    if (!grade_found) {
        fprintf(stderr, "User's grade not found.\n"); // Error if grade not found
        return;
    }

    printf("Current user's grade: %s\n", user_grade); // Debug print

    *numFilteredResults = 0;  // Initialize the number of filtered results

    // Loop through all files to filter results based on grade compatibility
    for (int i = 0; i < numFiles; i++) {
        char edu_file_path[100];
        snprintf(edu_file_path, sizeof(edu_file_path), "Databases/Edu_data/%s.csv", education[i].name); // Construct file path
        printf("Attempting to open education file: %s\n", edu_file_path);

        FILE* edu_file = fopen(edu_file_path, "r"); // Open education file
        if (edu_file == NULL) {
            perror("Opening education file failed"); // Error handling
            fprintf(stderr, "Failed to open %s.\n", edu_file_path);
            continue; // Skip to next file
        }
        printf("Opened %s successfully.\n", edu_file_path);

        char edu_line[MAX_LEN];
        bool isCompatible = false;
        while (fgets(edu_line, sizeof(edu_line), edu_file)) {
            printf("Reading line from education file: %s", edu_line);
            char* req_grade = strrchr(edu_line, ',') + 1; // Extract required grade

            // Remove newline character from req_grade
            req_grade[strcspn(req_grade, "\r\n")] = 0;

            printf("Comparing required grade '%s' with user grade '%s'\n", req_grade, user_grade);

            // Check grade compatibility
            if (req_grade && isGradeCompatible(req_grade, user_grade)) {
                isCompatible = true;
                printf("Grade is compatible.\n");
                break; // Break if compatible
            } else {
                printf("Grade is not compatible.\n");
            }
        }
        fclose(edu_file); // Close education file

        if (isCompatible) {
            // If grade is compatible, add to filtered results
            filteredResults[*numFilteredResults] = result[i];
            (*numFilteredResults)++; // Increment count of filtered results
        }
    }

    // Copy filtered results back to the original array
    for (int i = 0; i < *numFilteredResults; i++) {
        result[i] = filteredResults[i];
    }

    printf("Completed filter_results with %d compatible results.\n", *numFilteredResults);
}

