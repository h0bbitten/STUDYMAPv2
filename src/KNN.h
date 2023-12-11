#include <stdio.h>

#define NUM_EDU 8
#define NUM_ANSWER 9

extern char* result_path;
extern int file_count;

typedef struct KnnDataPoints {

    int answers[NUM_ANSWER];
    char* name;
    double result;

} KnnDataPoints;


void knn();
void parse_data(FILE* data_stream, KnnDataPoints* KnnDataPoint);
double Euclidean_distance(KnnDataPoints KnnTrainingPoint, KnnDataPoints KnnUserPoint);
int smallest_value(const void *a, const void *b);
void cleanup(KnnDataPoints* KnnDataPoint);
