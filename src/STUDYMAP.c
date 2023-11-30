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
    /* TODO:
     *  1. Unique username DOOOOOOOOOONE
     *  2. Password hashing (Open SSL)
     *  3. Tilføj "cpr-nummer" til bruger struct DOOOOOOOOONE
    */
    //Add_user(...)
    //***OR***
    //Load_profile(...)

    //After getting the user profile, the program will get the education data from a remote database
    //This is simulated by getting data from a csv file
    //Load_data(...)

    /* TODO:
     *  1. Mockup karakterblad ("CPR-nummer", karakter1...karakter_n, uddannelsested)
     *  2. Hvis brugeren ikke har ungdomsuddannelse så lad bruger vælge en
     *
     */
    //User having a lack of secondary education will return an error and log out the user

    //Is there previously saved results?
    //***YES***
    //The user can now either start a new test or load previous results
    Questionnaire();
    /* TODO:
     *  1. Insert/update questions - Jonas!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
     *  2. Questionnaire progress
     *  3. Dato og navn til questionnaire
     *
     *  */

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

return 0;
}





