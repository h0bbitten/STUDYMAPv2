#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Results.h"
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

void read_results(char* file_path, results* result);

void read_edu_data(char* file_path, educations education[8]);

void filter_results(results* result);

void display_results(results* result);


void Display_results() {
    char edu_data[] = "Databases/Uni.csv";


    int Edu_total = file_count;

    results result[8];

    educations educationEd];

    read_results(result_path, &result[8]);

    read_edu_data(edu_data, &education[8]);

    filter_results(&result[8]);

    int k = 3;

    for (int i = 0; i < k; i++) {
        display_results(&result[k]);
    }

}

void read_results(char* file_path, results* result){


    //kan filen åbne?
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        printf("Kunne ikke åbne %s.\n", file_path);
        return;
    }

    char line[MAX_LEN];

    while (fgets(line, sizeof(line), file)) {
        char *token = strtok(line, DELIMITER);

            if (token != NULL) {
                strcpy(result->name, token);
                token = strtok(line, DELIMITER);
                if (token != NULL) {
                    result->value = atoi(token);
                    printf("%s, %f", result->name, result->value);
                }
            }
    }

    fclose(file);
}
