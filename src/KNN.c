#include "questionnaire.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

char* answers_path;
char* datast_path = {"Databases/datast.csv"};

typedef struct KnnDataPoints {

    int answers[8];
    char* name;
    double result;

} KnnDataPoints;

void parse_data (KnnDataPoints KnnTrainingPoint[8]);
void parse_data_input (KnnDataPoints *KnnUserPoint);
double Euclidean_distance(KnnDataPoints KnnTrainingPoint, KnnDataPoints KnnUserPoint);
int smallest_value(const void *a, const void *b);

void knn() {

    KnnDataPoints KnnTrainingPoint[8];

    parse_data (KnnTrainingPoint);

    KnnDataPoints KnnUserPoint;
    parse_data_input (&KnnUserPoint);

    for (int i = 0; i < 4; i++) {
        KnnTrainingPoint[i].result = Euclidean_distance(KnnTrainingPoint[i], KnnUserPoint);
    }

    qsort(KnnTrainingPoint, 4, sizeof(KnnDataPoints), smallest_value);


    int k = 3;

    printf("%s\n", KnnUserPoint.name);
    for (int i = 0; i < k; i++) {
        printf("%s: %f\n", KnnTrainingPoint[i].name, KnnTrainingPoint[i].result);
    }

}

void parse_data (KnnDataPoints KnnTrainingPoint[8]) {

    KnnTrainingPoint[0].name = "Computer Science";
    KnnTrainingPoint[0].answers[0] = 3;
    KnnTrainingPoint[0].answers[1] = 8;
    KnnTrainingPoint[0].answers[2] = 10;
    KnnTrainingPoint[0].answers[3] = 7;
    KnnTrainingPoint[0].answers[4] = 10;
    KnnTrainingPoint[0].answers[5] = 10;
    KnnTrainingPoint[0].answers[6] = 8;
    KnnTrainingPoint[0].answers[7] = 5;

    KnnTrainingPoint[1].name = "Computer Technology";
    KnnTrainingPoint[1].answers[0] = 3;
    KnnTrainingPoint[1].answers[1] = 8;
    KnnTrainingPoint[1].answers[2] = 10;
    KnnTrainingPoint[1].answers[3] = 8;
    KnnTrainingPoint[1].answers[4] = 7;
    KnnTrainingPoint[1].answers[5] = 8;
    KnnTrainingPoint[1].answers[6] = 8;
    KnnTrainingPoint[1].answers[7] = 5;

    KnnTrainingPoint[2].name = "Data Science and ML";
    KnnTrainingPoint[2].answers[0] = 5;
    KnnTrainingPoint[2].answers[1] = 6;
    KnnTrainingPoint[2].answers[2] = 8;
    KnnTrainingPoint[2].answers[3] = 10;
    KnnTrainingPoint[2].answers[4] = 8;
    KnnTrainingPoint[2].answers[5] = 8;
    KnnTrainingPoint[2].answers[6] = 10;
    KnnTrainingPoint[2].answers[7] = 7;

    KnnTrainingPoint[2].name = "Information Technology";
    KnnTrainingPoint[2].answers[0] = 8;
    KnnTrainingPoint[2].answers[1] = 4;
    KnnTrainingPoint[2].answers[2] = 7;
    KnnTrainingPoint[2].answers[3] = 3;
    KnnTrainingPoint[2].answers[4] = 6;
    KnnTrainingPoint[2].answers[5] = 7;
    KnnTrainingPoint[2].answers[6] = 5;
    KnnTrainingPoint[2].answers[7] = 2;

    for (int i = 0; i < 4; i++) {
        KnnTrainingPoint[i].result = 0.0;
    }

}
void parse_data_input (KnnDataPoints *KnnUserPoint) {

    KnnUserPoint->name = "Allan";
    KnnUserPoint->answers[0] = 3;
    KnnUserPoint->answers[1] = 8;
    KnnUserPoint->answers[2] = 10;
    KnnUserPoint->answers[3] = 7;
    KnnUserPoint->answers[4] = 10;
    KnnUserPoint->answers[5] = 10;
    KnnUserPoint->answers[6] = 8;
    KnnUserPoint->answers[7] = 5;

    KnnUserPoint->result = 0.0;
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