#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

    for (int i = 0; i < file_count; i++) {
        // Check if the value is not filtered
        if (result[i].value != -1.0) {
            for (int j = 0; j < file_count; j++) {
                // Find the matching education description
                if (strcmp(result[i].name, education[j].name) == 0) {
                    // Calculate the match percentage
                    double percentage = ((1 - (result[i].value / ref_distance)) * 100);

                    // Print the result
                    printf("%s %.2f%% match:\n%s\n", result[i].name, percentage, education[j].description);
                    break;  // No need to check further for this result
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

        token = strtok(NULL, DELIMITER);
        strcpy(current_user.password, token);

        token = strtok(NULL, DELIMITER);
        strcpy(current_user.cpr, token);

        token = strtok(NULL, DELIMITER);
        strcpy(current_user.requirements, token);
    }

    fclose(file);
}


void filter_results(results result[8], educations education[8]) {
    for (int i = 0; i < file_count; i++) {
        char edu_file_name[100];  // Adjust buffer size as needed

        // Construct the file name with the correct format
        snprintf(edu_file_name, sizeof(edu_file_name), "Databases/Edu_data/%s.csv", education[i].name);

        // Open the education file
        FILE* edu_file = fopen(edu_file_name, "r");
        if (edu_file == NULL) {
            fprintf(stderr, "Failed to open %s.\n", edu_file_name);
            continue;  // Skip to the next education file if it doesn't exist
        }

        // Read the entire file into memory
        fseek(edu_file, 0, SEEK_END);
        long file_size = ftell(edu_file);
        rewind(edu_file);
        char* file_content = malloc(file_size + 1);
        fread(file_content, 1, file_size, edu_file);
        file_content[file_size] = '\0';

        char* token;

        // Tokenize the file content
        token = strtok(file_content, DELIMITER);
        while (token != NULL) {
            // Check if the token is "done"
            if (strcmp(token, "done") == 0) {
                // Check if the next token is "Mat A"
                token = strtok(NULL, DELIMITER);
                if (token != NULL && strcmp(token, "MatA") == 0) {
                    // Check if the user has "Mat B"
                    if (strstr(current_user.requirements, "MatB") != NULL) {
                        // Filter out the result
                        printf("Filtering out result: %s\n", result[i].name);
                        result[i].value = -1.0;  // Placeholder for filtering
                        break;  // No need to check further for this education
                    }
                }
            }

            // Move to the next token
            token = strtok(NULL, DELIMITER);
        }

        // Free allocated memory
        free(file_content);
        fclose(edu_file);
    }
}


