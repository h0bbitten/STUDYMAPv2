#include <stdio.h>
#include <stdlib.h>
#include "data_collection.h"
#include "load_profile.h"
#include "questionnaire.h"
#include "KNN.h"

#include <math.h>

int main() {
    //The program boots and the user is prompted to login with "MY-ID"
    //Load_user(...)
    login();
    //Add_user(...)
    //***OR***
    Load_profile();

    //After getting the user profLile, the program will get the education data from a remote database
    //This is simulated by getting data from a csv file
    //Load_data(...)

    //Is there previously saved results?
    //***YES***
    //The user can now either start a new test or load previous results
    Questionnaire();

    //KNN algorithm(...) - SØREN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    //Process_data(...)
    //Plot_educations(...)
    //Plot_result(...)
    //Calculate_distance(...)

    //Display_result(...) - Frederik!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    //Change number of recommendations or decide a different action
    //Results(...)
    //Saved_results(...)

    //***NO***
    //Saved_results(...)
    knn();

return 0;
}





