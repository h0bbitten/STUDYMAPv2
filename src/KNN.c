#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_USERS 100
#define MAX_QUESTIONS 9

// Structure to store user data
struct UserData {
    char username[50];
    double features[MAX_QUESTIONS];
};

// Function to calculate Euclidean distance between two sets of features
double calculateDistance(double *features1, double *features2, int numFeatures) {
    double distance = 0.0;

    for (int i = 0; i < numFeatures; ++i) {
        distance += pow(features1[i] - features2[i], 2);
    }

    return sqrt(distance);
}

// Function to find k-nearest neighbors
void findKNN(struct UserData *dataset, int dataSize, double *allanFeatures, int k) {
    // Calculate distances and store them in an array
    double distances[MAX_USERS];
    for (int i = 0; i < dataSize; ++i) {
        distances[i] = calculateDistance(dataset[i].features, allanFeatures, MAX_QUESTIONS);
    }

    // Find and print the top k distances
    printf("Top %d matches:\n", k);
    for (int i = 0; i < k; ++i) {
        double minDistance = distances[0];
        int minIndex = 0;

        for (int j = 1; j < dataSize; ++j) {
            if (distances[j] < minDistance) {
                minDistance = distances[j];
                minIndex = j;
            }
        }

        printf("Match %d: %s\n", i + 1, dataset[minIndex].username);

        // Set the distance to a large value to avoid re-selection
        distances[minIndex] = INFINITY;
    }
}

int knn() {
    // Read dataset from datast.csv
    FILE *dataFile = fopen("datast.csv", "r");
    if (!dataFile) {
        perror("Error opening datast.csv");
        return 1;
    }

    struct UserData dataset[MAX_USERS];
    int dataSize = 0;

    // Read data from the CSV file
    while (fscanf(dataFile, "\"%49[^\"]\",%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n",
                  dataset[dataSize].username,
                  &dataset[dataSize].features[0], &dataset[dataSize].features[1],
                  &dataset[dataSize].features[2], &dataset[dataSize].features[3],
                  &dataset[dataSize].features[4], &dataset[dataSize].features[5],
                  &dataset[dataSize].features[6], &dataset[dataSize].features[7]) == 9) {
        dataSize++;
    }

    fclose(dataFile);

    // Read Allan file from allan.csv
    FILE *allanFile = fopen("allan.csv", "r");
    if (!allanFile) {
        perror("Error opening allan.csv");
        return 1;
    }

    char username[50];
    double allanFeatures[MAX_QUESTIONS];

    // Read user data from the CSV file
    while (fscanf(allanFile, "%49[^,],%*[^,],%49[^,],%lf,%49[^,],%lf,%49[^,],%lf,%49[^,],%lf,%49[^,],%lf,%49[^,],%lf,%49[^,],%lf,%49[^,],%lf,%49[^,],%lf,%49[^,],%lf,%*[^,],\n",
                  username,
                  &allanFeatures[0], &allanFeatures[1],
                  &allanFeatures[2], &allanFeatures[3],
                  &allanFeatures[4], &allanFeatures[5],
                  &allanFeatures[6], &allanFeatures[7]) == 9) {

        printf("\nUsername: %s\n", username);
        findKNN(dataset, dataSize, allanFeatures, 3);
    }

    fclose(allanFile);

    return 0;
}
