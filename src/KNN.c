#include "Registration.h"
#include "Main_menu.h"
#include "Questionnaire.h"
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

void KNN() {

    file_names files[MAX_FILES];
    int file_count = 0;

    // Scan file names and assign a number to each file
    scan_file_names(edu_data_dir_path, files, &file_count);

    // Create structs for knn points
    knn_data_points knn_training_point[file_count];
    knn_data_points knn_user_point;

    // Loop that runs for the amount of educations
    for (int i = 0; i < file_count; i++) {
        // Assigns the name of the education to the struct
        knn_training_point[i].name = strdup(files[i].name);
        char data_path[PATH_MAX];

        // Creates path for education
        snprintf(data_path, sizeof(data_path), "Databases/Edu_data/%s.csv", knn_training_point[i].name);

        // Parse data from education answers to KNN
        FILE* data_file = fopen(data_path, "r");
        parse_data(data_file, &knn_training_point[i]);
        fclose(data_file);
    }

    // Parse data from current users answers to KNN
    FILE* answer;
    answer = fopen(answers_path, "r");
    parse_data(answer, &knn_user_point);

    fclose(answer);

    // Calculate distance between all educations and answers
    for (int i = 0; i < file_count; i++) {
        knn_training_point[i].result = euclidean_distance(knn_training_point[i], knn_user_point);
    }

    // Sorts the distances smallest values first
    qsort(knn_training_point, file_count, sizeof(knn_data_points), smallest_value);

    // Create directory for results
    make_directory("Databases/Results");

    // Create path for directory for results for current user
    char *dir_results_path;
    dir_results_path = (char*)malloc(PATH_MAX);
    if (!dir_results_path) {
        fprintf(stderr, "Error allocating memory for dir_results_path.\n");
    }
    snprintf(dir_results_path, PATH_MAX, "Databases/Results/%s", current_user.username);

    // Create directory for results for current user
    make_directory(dir_results_path);

    // Create path for results for current user and current questionnaire
    result_path = (char*)malloc(PATH_MAX);
    if (!result_path) {
        fprintf(stderr, "Error allocating memory for result_path.\n");
    }
    snprintf(result_path, PATH_MAX, "%s/%s.csv",dir_results_path, the_time);

    // Write to result file
    FILE *Result;
    Result = fopen(result_path, "a");

    // Create file for user results if it doesn't exist
    if(Result == NULL)
    {
        Result = fopen(result_path, "w");
    }

    // Write top k results to file (k = file_count)
    for (int i = 0; i < file_count; i++) {
        fprintf(Result, "%s,%f\n", knn_training_point[i].name, knn_training_point[i].result);
    }

    fclose(Result);

    // Clean up allocated memory
    for (int i = 0; i < file_count; i++) {
        cleanup(&knn_training_point[i]);
    }

}
// Function to parse data from file with the "saved_answers" format
void parse_data(FILE* data_stream, knn_data_points* knn_data_point) {
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
                knn_data_point->answers[(index - 1) / 2] = atoi(token);
            }
        }

        // Move to the next token
        token = strtok(NULL, ",");
        index++;
    }

    // Initialize result
    knn_data_point->result = 0.0;

}
// Function to calculate the Euclidean distance between two points
double euclidean_distance(knn_data_points knn_training_point, knn_data_points knn_user_point){

    // Calculates a part of Euclidean's distance formula for each answer
    double distance = 0.0;
    for (int i = 0; i < NUM_ANSWER; i++) {
        distance += pow(knn_user_point.answers[i] - knn_training_point.answers[i], 2);
    }
    // Returns the completely calculated result for all answers
    return sqrt(distance);
}
// Function to find the find the smallest value between two numbers
int smallest_value(const void *a, const void *b) {

    // A q-sort compare function that compares the value of results
    double result_A = ((knn_data_points *)a)->result;
    double result_B = ((knn_data_points *)b)->result;

    if (result_A < result_B) return -1;
    else if (result_A > result_B) return 1;
    else return 0;
}
// Function to free allocated memory for names
void cleanup(knn_data_points* knn_data_point) {
    // Frees' memory for each name
    free(knn_data_point->name);
}