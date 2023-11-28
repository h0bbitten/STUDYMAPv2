// KNN.h

#ifndef KNN_H
#define KNN_H

#define K 3

typedef struct {
    int predictedClass;
    int nearestNeighborsIndices[K];
} ClassificationResult;

double calculateDistance(double *point1, double *point2, int numFeatures);
int findMinIndex(double *arr, int size);
ClassificationResult knn(double **trainingData, double *newData, int numTrainingSamples, int numFeatures);
double **readTrainingData(const char *filename, int *numTrainingSamples, int *numFeatures);
void freeTrainingData(double **trainingData, int numTrainingSamples);

#endif // KNN_H
