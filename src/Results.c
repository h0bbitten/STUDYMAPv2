#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Results.h"
#include "Registration.h"
#include "load_profile.h"
#include "KNN.h"

typedef struct {
    char* name;
    double value;

} results;

typedef struct {
    char* name;
    char* link;
    char* description;

} educations;

int file_count = 8;


void read_results(char* file_path, results result[8]);

void read_edu_data(char* file_path, educations education[8]);

void filter_results(results result[8]);

void print_results(results result[8], educations education[8], int num_to_print);


void Display_results() {
    char* edu_data = "Databases/Uni.csv";

    results result[file_count];

    educations education[file_count];

    read_results(result_path, result);

    read_edu_data(edu_data, education);

    //filter_results(result);

    int k = 3;

    printf("These are the top %d recommended results for %s:\n\n", k, current_user.username);
    print_results(result, education, k);

    // Free allocated memory
    for (int i = 0; i < file_count; i++) {
        free(result[i].name);
        free(education[i].name);
        free(education[i].link);
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
                education[count].link = malloc(strlen(token) + 1);
                strcpy(education[count].link, token);

                token = strtok(NULL, DELIMITER);
                if (token != NULL) {
                    education[count].description = malloc(strlen(token) + 1);
                    strcpy(education[count].description, token);

                    count++;
                }
            }
        }
    }

    fclose(file);

}
void print_results(results result[8], educations education[8], int num_to_print) {

    double ref_distance = 27.0;

    for (int i = 0; i < num_to_print; i++) {
        for (int j = 0; j < file_count; j++) {
            if (strcmp(result[i].name, education[j].name) == 0) {

                double percentage = ((1 - (result[i].value / ref_distance)) * 100);

                printf("%s (%.2f%% match):\n%s\nTo see more check out: %s\n\n", result[i].name, percentage, education[j].description, education[j].link);
            }
        }
    }
}
