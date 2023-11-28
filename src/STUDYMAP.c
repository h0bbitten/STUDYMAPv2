#include <stdio.h>
#include <stdlib.h>
#include "data_collection.h"
#include "questionnaire.h"
#include "KNN.h"
#include <math.h>


int main() {
    //The program boots and the user is prompted to login with "MY-ID"
    //Load_user(...)
    login();

    //Add_user(...)
    //***OR***
    //Load_profile(...)

    //After getting the user profile, the program will get the education data from a remote database
    //This is simulated by getting data from a csv file
    //Load_data(...)
    //User having a lack of secondary education will return an error and log out the user

    //Is there previously saved results?
    //***YES***
    //The user can now either start a new test or load previous results
    Questionnaire();

    //Three_dimensional(...)
    //KNN(...)
    //SVM(...)

    //Process_data(...)
    //Plot_educations(...)
    //Plot_result(...)
    //Calculate_distance(...)

    //Display_result(...)

    //Change number of recommendations or decide a different action
    //Results(...)
    //Saved_results(...)

    //***NO***
    //Saved_results(...)

    //KNN part of main

    // Read training data from Answers_test.csv
    int numTrainingSamples, numFeatures;
    double **trainingData = readTrainingData("Answers_test.csv", &numTrainingSamples, &numFeatures);

    // Assume newData is a double array representing a new data point from recommendation.csv
    double newData[numFeatures];
    // Initialize newData with appropriate values
    // Call knn to get the classification result
    // Assuming you have already read trainingData and newData
    ClassificationResult result = knn(trainingData, newData, numTrainingSamples, numFeatures);


    // Print the top K predicted classes and their nearest neighbors
    for (int i = 0; i < K; ++i) {
        // Print the predicted class and its nearest neighbors
        printf("Predicted Class: %d\n", result.predictedClass);
        printf("Nearest Neighbors Indices: ");
        for (int j = 0; j < K; ++j) {
            printf("%d ", result.nearestNeighborsIndices[j]);
        }
        printf("\n");

    // Free allocated memory for training data
        freeTrainingData(trainingData, numTrainingSamples);


        return 0;

    }
}