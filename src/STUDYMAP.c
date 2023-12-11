#include <stdio.h>
#include <stdlib.h>
#include "data_collection.h"
#include "load_profile.h"
#include "questionnaire.h"
#include "KNN.h"
#include "Results.h"

#include <math.h>

int main() {
    //The program boots and the user is prompted to login with "MY-ID"
    //Load_user(...)
    login();
    //Add_user(...)
    //***OR***
    bool do_questionnaire;
    Load_profile(&do_questionnaire);

    //After getting the user profLile, the program will get the education data from a remote database
    //This is simulated by getting data from a csv file
    //Load_data(...)

    //Is there previously saved results?
    //***YES***
    //The user can now either start a new test or load previous results
    if(do_questionnaire == true){
        Questionnaire();
        knn();
    }

    //Filter_results

    Display_results();

    //Change number of recommendations or decide a different action
    //Results(...)
    //Saved_results(...)

    //***NO***
    //Saved_results(...)

return 0;
}





