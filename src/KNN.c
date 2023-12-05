#include "questionnaire.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define NUM_EDU 8
#define NUM_ANSWER 8

char* answers_path;
char* datast_path = {"Databases/datast.csv"};

typedef struct KnnDataPoints {

    int answers[NUM_ANSWER];
    char* name;
    double result;

} KnnDataPoints;

void parse_data(FILE* data_stream, KnnDataPoints* KnnDataPoint);
double Euclidean_distance(KnnDataPoints KnnTrainingPoint, KnnDataPoints KnnUserPoint);
int smallest_value(const void *a, const void *b);
void cleanup(KnnDataPoints* KnnDataPoint);

void knn() {

    KnnDataPoints KnnTrainingPoint[NUM_EDU];

    FILE* datast;
    datast = fopen(datast_path, "r");

    for (int i = 0; i < NUM_EDU; ++i) {
        parse_data(datast, &KnnTrainingPoint[i]);
    }

    fclose(datast);


    FILE* answer;
    //answer = fopen(answers_path, "r");
    answer = fopen("Databases/Answers/Q.csv", "r");

    KnnDataPoints KnnUserPoint;
    parse_data(answer, &KnnUserPoint);

    fclose(answer);

    for (int i = 0; i < NUM_EDU; i++) {
        KnnTrainingPoint[i].result = Euclidean_distance(KnnTrainingPoint[i], KnnUserPoint);
    }

    qsort(KnnTrainingPoint, 4, sizeof(KnnDataPoints), smallest_value);


    int k = 3;

    printf("%s\n", KnnUserPoint.name);
    for (int i = 0; i < k; i++) {
        printf("%s: %f\n", KnnTrainingPoint[i].name, KnnTrainingPoint[i].result);
    }

    // Clean up allocated memory
    for (int i = 0; i < NUM_EDU; i++) {
        cleanup(&KnnTrainingPoint[i]);
    }
    cleanup(&KnnUserPoint);

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

        // The first token is the name
        if (index == 0) {
            KnnDataPoint->name = malloc(strlen(token) + 1);  // +1 for the null terminator
            if (KnnDataPoint->name != NULL) {
                strcpy(KnnDataPoint->name, token);
                token = strtok(NULL, ",");
                token = strtok(NULL, ",");
            }
        } else if (index % 2 == 1) {
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

    double distance = 0.0;


    for (int i = 0; i < 8; i++) {
        distance += pow(KnnUserPoint.answers[i] - KnnTrainingPoint.answers[i], 2);
    }

    return sqrt(distance);
}
int smallest_value(const void *a, const void *b) {
    double result_A = ((KnnDataPoints *)a)->result;
    double result_B = ((KnnDataPoints *)b)->result;

    if (result_A < result_B) return -1;
    else if (result_A > result_B) return 1;
    else return 0;
}
void cleanup(KnnDataPoints* KnnDataPoint) {
    free(KnnDataPoint->name);
}