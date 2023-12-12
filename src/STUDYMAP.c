#include <stdio.h>
#include <stdlib.h>
#include "data_collection.h"
#include "load_profile.h"
#include "questionnaire.h"
#include "KNN.h"
#include "Results.h"

#include <math.h>

int main() {

    login();

    bool do_questionnaire;
    Load_profile(&do_questionnaire);


    if(do_questionnaire == true){
        Questionnaire();
        knn();
    }

    Display_results();


return 0;
}





