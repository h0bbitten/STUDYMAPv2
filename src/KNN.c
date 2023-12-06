#include "questionnaire.h"
#include "KNN.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#if defined(_WIN32) || defined(_WIN64)
#include <direct.h>
#define mkdir(path, mode) _mkdir(path)
#elif defined(__APPLE__)
#include <sys/stat.h>
    #include <unistd.h>
#else
    #include <sys/stat.h>
    #include <sys/types.h>
#endif


char* answers_path;
char* datast_path = {"Databases/datast.csv"};
char* result_path;

int make_directory(const char *path) {
#if defined(_WIN32) || defined(_WIN64)
    if (_mkdir(path) == 0) {
        printf("Directory created successfully.\n");
        return 0;
    } else {
        printf("Failed to create directory.\n");
        return -1;
    }
#elif defined(__APPLE__)
    if (mkdir(path, 0777) == 0) {
            printf("Directory created successfully.\n");
            return 0;
        } else {
            printf("Failed to create directory.\n");
            return -1;
        }
    #else
        if (mkdir(path, 0777) == 0) {
            printf("Directory created successfully.\n");
            return 0;
        } else {
            printf("Failed to create directory.\n");
            return -1;
        }
#endif
}

void knn() {

    const char *dirname = "Databases/Test";

    if (make_directory(dirname) == 0) {
        printf("Directory created successfully.\n");
    } else {
        printf("Failed to create directory.\n");
    }



    KnnDataPoints KnnTrainingPoint[NUM_EDU];

    FILE* datast;
    datast = fopen(datast_path, "r");

    //Parse data from datast to KNN
    for (int i = 0; i < NUM_EDU; ++i) {
        parse_data(datast, &KnnTrainingPoint[i]);
    }

    fclose(datast);


    FILE* answer;
    answer = fopen(answers_path, "r");
    //answer = fopen("Databases/Answers/Q.csv", "r");

    KnnDataPoints KnnUserPoint;
    //Parse data from current users answers to KNN
    parse_data(answer, &KnnUserPoint);

    fclose(answer);

    //Calculate distance between all educations and answers
    for (int i = 0; i < NUM_EDU; i++) {
        KnnTrainingPoint[i].result = Euclidean_distance(KnnTrainingPoint[i], KnnUserPoint);
    }

    //Sorts the distances smallest values first
    qsort(KnnTrainingPoint, 4, sizeof(KnnDataPoints), smallest_value);

    //Top k nearest neighbors to return
    int k = 3;

    //Displays results to user, probably should be moved from the KNN function
    printf("Top %d recommended educations for %s;\n\n", k, KnnUserPoint.name);
    for (int i = 0; i < k; i++) {
        printf("%s: %f\n", KnnTrainingPoint[i].name, KnnTrainingPoint[i].result);
    }

    //Create path for results for current user
    result_path = (char*)malloc(PATH_MAX);
    if (!result_path) {
        fprintf(stderr, "Error allocating memory for result_path.\n");
    }
    snprintf(result_path, PATH_MAX, "Databases/Results/%s.csv", KnnUserPoint.name);

    //Write to result file
    FILE *Result;
    Result = fopen(result_path, "a");

    //Create file for user results if it doesn't exist
    if(Result == NULL)
    {
        Result = fopen(result_path, "w");
    }

    //Write top k results to file
    for (int i = 0; i < k; i++) {
        fprintf(Result, "%s,%f\n", KnnTrainingPoint[i].name, KnnTrainingPoint[i].result);
    }

    fclose(Result);

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

    //Calculates a part of Euclidean's distance formula for each answer
    double distance = 0.0;
    for (int i = 0; i < 8; i++) {
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