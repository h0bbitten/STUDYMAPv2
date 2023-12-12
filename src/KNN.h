#include <stdio.h>

#define NUM_ANSWER 9

extern char* result_path;

typedef struct KnnDataPoints {

    int answers[NUM_ANSWER];
    char* name;
    double result;

} knn_data_points;


void KNN();
void parse_data(FILE* data_stream, knn_data_points* knn_data_point);
double euclidean_distance(knn_data_points knn_training_point, knn_data_points knn_user_point);
int smallest_value(const void *a, const void *b);
void cleanup(knn_data_points* knn_data_point);
