#include "Registration.h"
#include "load_profile.h"
#include "questionnaire.h"
#include "KNN.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>


char* answers_path;
char* edu_data_dir_path = {"Databases/Edu_data"};
char* result_path;

void knn() {

    file_names files[MAX_FILES];
    int file_count = 0;

    // Scan file names and assign a number to each file
    scan_file_names(edu_data_dir_path, files, &file_count);

    KnnDataPoints KnnTrainingPoint[file_count];


    // Display all the files in the directory
    for (int i = 0; i < file_count; i++) {
        KnnTrainingPoint[i].name = strdup(files[i].name);
        char data_path[PATH_MAX];

        snprintf(data_path, sizeof(data_path), "Databases/Edu_data/%s.csv", KnnTrainingPoint[i].name);

        FILE* data_file = fopen(data_path, "r");
        parse_data(data_file, &KnnTrainingPoint[i]);
        fclose(data_file);
    }

    FILE* answer;
    answer = fopen(answers_path, "r");

    KnnDataPoints KnnUserPoint;
    //Parse data from current users answers to KNN
    parse_data(answer, &KnnUserPoint);

    fclose(answer);

    //Calculate distance between all educations and answers
    for (int i = 0; i < file_count; i++) {
        KnnTrainingPoint[i].result = Euclidean_distance(KnnTrainingPoint[i], KnnUserPoint);
    }

    //Sorts the distances smallest values first
    qsort(KnnTrainingPoint, file_count, sizeof(KnnDataPoints), smallest_value);

    //Create directory for results
    make_directory("Databases/Results");

    //Create path for directory for results for current user
    char *dir_results_path;
    dir_results_path = (char*)malloc(PATH_MAX);
    if (!dir_results_path) {
        fprintf(stderr, "Error allocating memory for dir_results_path.\n");
    }
    snprintf(dir_results_path, PATH_MAX, "Databases/Results/%s", current_user.username);

    //Create directory for results for current user
    make_directory(dir_results_path);

    //Create path for results for current user and current questionnaire
    result_path = (char*)malloc(PATH_MAX);
    if (!result_path) {
        fprintf(stderr, "Error allocating memory for result_path.\n");
    }
    snprintf(result_path, PATH_MAX, "%s/%s.csv",dir_results_path, the_time);

    //Write to result file
    FILE *Result;
    Result = fopen(result_path, "a");

    //Create file for user results if it doesn't exist
    if(Result == NULL)
    {
        Result = fopen(result_path, "w");
    }

    //Write top k results to file
    for (int i = 0; i < file_count; i++) {
        fprintf(Result, "%s,%f\n", KnnTrainingPoint[i].name, KnnTrainingPoint[i].result);
    }

    fclose(Result);

    // Clean up allocated memory
    for (int i = 0; i < file_count; i++) {
        cleanup(&KnnTrainingPoint[i]);
    }

}

void parse_data(FILE* data_stream, KnnDataPoints* KnnDataPoint) {
    char line[1024];
    char* token;

    // Read a line from the CSV file
    if (fgets(line, sizeof(line), data_stream) == NULL) {
        // Handle error or end of file
        return;
    }

    // Tokenize the line using comma as a delimiter
    token = strtok(line, ",");
    int index = 0;

    while (token != NULL) {
        if (strcmp(token, "done") == 0) {
            break;  // Stop when reaching "done"
        }

        if (index % 2 == 1) {
            // Process only numeric answers and skip non-numeric values
            if (isdigit(token[0])) {
                KnnDataPoint->answers[(index - 1) / 2] = atoi(token);
            }
        }

        // Move to the next token
        token = strtok(NULL, ",");
        index++;
    }

    // Initialize result
    KnnDataPoint->result = 0.0;

}

double Euclidean_distance(KnnDataPoints KnnTrainingPoint, KnnDataPoints KnnUserPoint){

    //Calculates a part of Euclidean's distance formula for each answer
    double distance = 0.0;
    for (int i = 0; i < NUM_ANSWER; i++) {
        distance += pow(KnnUserPoint.answers[i] - KnnTrainingPoint.answers[i], 2);
    }
    //Returns the completely calculated result for all answers
    return sqrt(distance);
}
int smallest_value(const void *a, const void *b) {

    //A q-sort compare function that compares the value of results
    double result_A = ((KnnDataPoints *)a)->result;
    double result_B = ((KnnDataPoints *)b)->result;

    if (result_A < result_B) return -1;
    else if (result_A > result_B) return 1;
    else return 0;
}
void cleanup(KnnDataPoints* KnnDataPoint) {
    //Frees' memory for each name, beause I had to use malloc to allocate memory for them, not sure why it's needed but program crashes without :shrug:
    free(KnnDataPoint->name);
}