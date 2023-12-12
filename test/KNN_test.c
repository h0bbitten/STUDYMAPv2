#include "KNN.h"
#include <assert.h>
#include <math.h>

/* Prototyper til test-funktioner */
void test_Euclidean_distance_case1(void);
void test_Euclidean_distance_case2(void);

/* Prototype til en funktion der sammenligner doubles men tillader små afrundingsfejl. */
int is_almost_equal(double, double);

int main(void) {
    /* Kør alle test-funktioner. */
    test_Euclidean_distance_case1();
    test_Euclidean_distance_case2();
}

double euclidean_distance(knn_data_points Knn_Training_Point, knn_data_points Knn_User_Point){

    //Calculates a part of Euclidean's distance formula for each answer
    double distance = 0.0;
    for (int i = 0; i < NUM_ANSWER; i++) {
        distance += pow(Knn_User_Point.answers[i] - Knn_Training_Point.answers[i], 2);
    }
    //Returns the completely calculated result for all answers
    return sqrt(distance);
}

void test_Euclidean_distance_case1(void) {
    double distance;
    knn_data_points TrainingData;
    knn_data_points UserData;

    TrainingData.answers[0] = 7;
    TrainingData.answers[1] = 4;
    TrainingData.answers[2] = 3;
    TrainingData.answers[3] = 2;
    TrainingData.answers[4] = 9;
    TrainingData.answers[5] = 9;
    TrainingData.answers[6] = 1;
    TrainingData.answers[7] = 5;
    TrainingData.answers[8] = 4;

    UserData.answers[0] = 6;
    UserData.answers[1] = 3;
    UserData.answers[2] = 9;
    UserData.answers[3] = 6;
    UserData.answers[4] = 10;
    UserData.answers[5] = 5;
    UserData.answers[6] = 2;
    UserData.answers[7] = 1;
    UserData.answers[8] = 8;

    distance = euclidean_distance(TrainingData, UserData);
    assert(is_almost_equal(distance, 10.19804));
}

void test_Euclidean_distance_case2(void) {
    double distance;
    knn_data_points TrainingData;
    knn_data_points UserData;

    TrainingData.answers[0] = 10;
    TrainingData.answers[1] = 10;
    TrainingData.answers[2] = 10;
    TrainingData.answers[3] = 10;
    TrainingData.answers[4] = 10;
    TrainingData.answers[5] = 10;
    TrainingData.answers[6] = 10;
    TrainingData.answers[7] = 10;
    TrainingData.answers[8] = 10;

    UserData.answers[0] = 10;
    UserData.answers[1] = 10;
    UserData.answers[2] = 10;
    UserData.answers[3] = 10;
    UserData.answers[4] = 10;
    UserData.answers[5] = 10;
    UserData.answers[6] = 10;
    UserData.answers[7] = 10;
    UserData.answers[8] = 10;

    distance = euclidean_distance(TrainingData, UserData);
    assert(is_almost_equal(distance, 0));
}



/* Due to the inaccuracies (rounding errors) of operations on floating-point number,
 * we allow a small error in our comparison between the actual and expected values. */
int is_almost_equal(double actual, double expected) {
    return fabs(actual - expected) < 0.000001;
}
