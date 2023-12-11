#include <stdio.h>
#include <stdlib.h>
#include "data_collection.h"
#include "load_profile.h"
#include "questionnaire.h"
#include "KNN.h"
#include "Results.h"
#include "Grade_Sheet.h"

int main() {
    // The program boots and the user is prompted to login with "MY-ID"
    // Load_user(...)
    login();
    // Add_user(...)
    // ***OR***
    bool do_questionnaire;
    Load_profile(&do_questionnaire);

    // After getting the user profile, the program will get the education data from a remote database
    // This is simulated by getting data from a CSV file
    // Load_data(...)

    // Is there previously saved results?
    // ***YES***
    // The user can now either start a new test or load previous results
    if (do_questionnaire == true) {
        Questionnaire();
        knn();
    }

    // Create a UserGradeData structure with the necessary data
    UserGradeData userGradeData;
    // Assume you have the user's CPR, courses, grades, and levels here

    // Provide the path to the education data CSV file
    char eduDataPath[] = "path_to_edu_data.csv";  // Replace with the actual path

    // Filter_results
    compareGrades(userGradeData, eduDataPath);
    Display_results();

    // Change number of recommendations or decide a different action
    // Results(...)
    // Saved_results(...)

    // ***NO***
    // Saved_results(...)

    return 0;
}
