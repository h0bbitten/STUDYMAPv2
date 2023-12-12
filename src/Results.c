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

    read_results(result_path, result);

    read_edu_data(edu_data, education);

   filter_results(result, education, numFiles);



    int k = 3;

    printf("These are the top %d recommended results for %s:\n\n", k, current_user.username);
    print_results(result, education, k);

    // Free allocated memory
    for (int i = 0; i < numFiles; i++) {
        free(result[i].name);
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
            if (strcmp(result[i].name, education[j].name) == 0) {

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
    if (strcmp(userGrade, "MatA") == 0) {
        return true; // MatA is compatible with anything
    } else if (strcmp(userGrade, "MatB") == 0) {
        return strcmp(requiredGrade, "MatB") == 0; // MatB is only compatible with MatB
    }
    return false; // Otherwise, not compatible
}

void filter_results(results* result, educations* education, int numFiles) {
    char user_grade[10];
    bool grade_found = false;
    char line[256];
    char* token;

    FILE* users_file = fopen("Databases/Users/Users.csv", "r");
    if (users_file == NULL) {
        fprintf(stderr, "Failed to open Users.csv.\n");
        return;
    }

    // Find the user's grade
    while (fgets(line, sizeof(line), users_file)) {
        token = strtok(line, ",");
        if (strcmp(token, current_user.username) == 0) {
            strtok(NULL, ",");  // Skip password and CPR
            token = strtok(NULL, ",\n");  // Read grade
            if (token) {
                strncpy(user_grade, token, sizeof(user_grade));
                grade_found = true;
                break;
            }
        }
    }
    fclose(users_file);

    if (!grade_found) {
        fprintf(stderr, "User's grade not found.\n");
        return;
    }

    // Filter results based on user's grade
    for (int i = 0; i < numFiles; i++) {
        char edu_file_path[100];
        snprintf(edu_file_path, sizeof(edu_file_path), "Databases/Edu_data/%s.csv", education[i].name);
        FILE* edu_file = fopen(edu_file_path, "r");
        if (edu_file == NULL) {
            fprintf(stderr, "Failed to open %s.\n", edu_file_path);
            result[i].value = -1.0; // Mark as filtered out
            continue;
        }

        char edu_line[MAX_LEN];
        bool isCompatible = false;
        while (fgets(edu_line, sizeof(edu_line), edu_file)) {
            char* req_grade = strrchr(edu_line, ',') + 1; // Get the grade requirement part
            if (req_grade && isGradeCompatible(req_grade, user_grade)) {
                isCompatible = true;
                break;
            }
        }
        fclose(edu_file);

        if (!isCompatible) {
            result[i].value = -1.0;  // Filter out this option
        }
    }
}