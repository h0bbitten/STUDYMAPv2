//
// Created by falseviking on 27-11-23.
//

#include "KNN.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_COLS 10
#define K 3


// Function to calculate Euclidean distance between two data points
double calculateDistance(double *point1, double *point2, int numFeatures) {
    double distance = 0.0;
    for (int i = 0; i < numFeatures; ++i) {
        distance += pow(point1[i] - point2[i], 2);
    }

    // Calculate the square root of the sum of squared distances
    return sqrt(distance);
}

// Function to find the index of the minimum element in an array
int findMinIndex(double *arr, int size) {
    int minIndex = 0;
    for (int i = 1; i < size; ++i) {
        if (arr[i] < arr[minIndex]) {
            minIndex = i;
        }
    }
    return minIndex;
}

// Function to perform KNN classification
ClassificationResult knn(double **trainingData, double *newData, int numTrainingSamples, int numFeatures) {
    // Calculate distances between newData and all training samples
    double distances[numTrainingSamples];
    for (int i = 0; i < numTrainingSamples; ++i) {
        distances[i] = calculateDistance(trainingData[i], newData, numFeatures);
    }

    // Find indices of K nearest neighbors
    int indices[K];
    for (int i = 0; i < K; ++i) {
        indices[i] = findMinIndex(distances, numTrainingSamples);
        distances[indices[i]] = INFINITY; // Mark this index as visited
    }

    // Perform a simple majority vote
    int classCounts[MAX_COLS];
    for (int i = 0; i < MAX_COLS; ++i) {
        classCounts[i] = 0;
    }

    // Find the class with the maximum count
    for (int i = 0; i < K; ++i) {
        int classIndex = (int)trainingData[indices[i]][numFeatures] - 1; // Subtract 1 from the class label value
        classCounts[classIndex]++;
    }

    int predictedClass = findMinIndex(classCounts, MAX_COLS);

    // Create a ClassificationResult struct to return the result
    ClassificationResult result;
    result.predictedClass = predictedClass;
    for (int i = 0; i < K; ++i) {
        result.nearestNeighborsIndices[i] = indices[i];
    }

    return result;
}

// Function to read training data from a CSV file
double **readTrainingData(const char *filename, int *numTrainingSamples, int *numFeatures) {
    FILE *trainingFile = fopen(filename, "r");
    if (trainingFile == NULL) {
        fprintf(stderr, "Error opening training file.\n");
        exit(1);
    }

    fscanf(trainingFile, "%d,%d", numTrainingSamples, numFeatures);

    double **trainingData = (double **)malloc(*numTrainingSamples * sizeof(double *));
    for (int i = 0; i < *numTrainingSamples; ++i) {
        trainingData[i] = (double *)malloc((*numFeatures + 1) * sizeof(double)); // +1 for class label
        for (int j = 0; j < *numFeatures + 1; ++j) {
            fscanf(trainingFile, "%lf,", &trainingData[i][j]);
        }
    }

    fclose(trainingFile);

    return trainingData;
}

// Function to free memory allocated for training data
void freeTrainingData(double **trainingData, int numTrainingSamples) {
    for (int i = 0; i < numTrainingSamples; ++i) {
        free(trainingData[i]);
    }
    free(trainingData);
}
