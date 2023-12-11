#include <stdio.h>
#include <stdlib.h>
#include "data_collection.h"
#include "load_profile.h"
#include "questionnaire.h"
#include "KNN.h"
#include "Results.h"

#include <math.h>

void what_now();

int main() {

    login();

    bool do_questionnaire;
    Load_profile(&do_questionnaire);

    if(do_questionnaire == true){
        Questionnaire();
        knn();
    }

    Display_results();

    what_now();

return 0;
}

/*

TODO
 Filter results!!!
 Change options for new test (not "666")     -       check
 Loop for whole program
 Options after displaying result (logout, new test, delete result, exit program, show more results, choose other save)
 Write description for each education
 Add more educations?
 Make results dynamic - not static using magic numbers
 Make functions same syntax, "example_function", not "ExampleFunction"
 Fix user experience, what's written out to the user
 Clean up code, remove unused stuff, don't break the program
 IDK, something else
 */

void what_now(){



    printf("\nChoose an option for what to do now\n");
}





