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

    // Read new data for recommendations from Recommendation.csv
    int numNewSamples, numNewFeatures;
    double **newData = readTrainingData("Recommendation.csv", &numNewSamples, &numNewFeatures);

    // Check if the number of features in the new data matches the training data
    if (numNewFeatures != numFeatures) {
        fprintf(stderr, "Error: Number of features in the new data does not match the training data.\n");
        exit(1);
    }

    // Perform KNN predictions for each set of new data
    int k = 3;  // Set the number of recommendations you want
    for (int i = 0; i < numNewSamples; ++i) {
        int recommendation = knn(trainingData, newData[i], numTrainingSamples, numFeatures);
        printf("Recommendation %d: %d\n", i + 1, recommendation);
    }

    // Free allocated memory
    freeTrainingData(trainingData, numTrainingSamples);
    freeTrainingData(newData, numNewSamples);

    return 0;
}