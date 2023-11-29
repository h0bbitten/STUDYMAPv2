// KNN.h
#include <math.h>

#ifndef STUDYMAP_KNN_H
#define STUDYMAP_KNN_H

// Declare the functions here
double calculateDistance(double *point1, double *point2, int numFeatures);
int findMinIndex(double *arr, int size);
int knn(double **trainingData, double *newData, int numTrainingSamples, int numFeatures);
double **readTrainingData(const char *filename, int *numTrainingSamples, int *numFeatures);
void freeTrainingData(double **trainingData, int numTrainingSamples);

#endif /* STUDYMAP_KNN_H */