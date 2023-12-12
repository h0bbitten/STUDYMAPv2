#include <stdio.h>

#define NUM_ANSWER 9

extern char* result_path;

typedef struct KnnDataPoints {

    int answers[NUM_ANSWER];
    char* name;
    double result;

} KnnDataPoints;


void KNN();
void parse_data(FILE* data_stream, KnnDataPoints* Knn_Data_Point);
double Euclidean_distance(KnnDataPoints Knn_Training_Point, KnnDataPoints Knn_User_Point);
int smallest_value(const void *a, const void *b);
void cleanup(KnnDataPoints* Knn_Data_Point);
