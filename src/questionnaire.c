#include "questionnaire.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


void Questionnaire(){

    //Check if questionaire file exists, if not creates one

    FILE *Answers;

    Answers = fopen("Answers.csv", "r");
    if(Answers == NULL)
    {
        Answers = fopen("Answers.csv", "w");
    }
    fclose(Answers);

    //start of the questionaire

    int number_of_questions = 1;

    //question 1
    int question_1_value;
    question("This is a test question", &number_of_questions);

    //question 2
    int question_2_value;
    question("Another test question!", &number_of_questions);
//penis
    //question 3
    //question();


    //question 4
    //question();

}

void question(char *prompt, int *question_number){
    bool valid_question = false;
    int question_value;
    //Prints the prompt
    printf("\n%s\n", prompt);
    printf("How much do you agree from 1 - 10\n>");

    //Keeps waiting for a valid integer value
    do {
        question_value = read_only_integer(&valid_question);
    } while (!valid_question || question_value <= 0 || question_value > 10);

    //Writes the question into the questionaire file
    printf("\nYou answered: %d\n", question_value);
    FILE *Answer;
    Answer = fopen("Answers.csv", "a");

    //Closes the questionaire file
    fclose(Answer);

    //counts up the question to the next one
    *question_number++;
}

int read_only_integer(bool *valid_question){
#define MAX_CHARACTERS 100

    char buffer[MAX_CHARACTERS];
    char *endptr;

    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        int number = strtol(buffer, &endptr, 10);

        if (buffer[0] != '\0' && (*endptr == '\n' || *endptr == '\0')) {
            *valid_question = true;
            return number;
        } else {
            return 0;
        }
    }
}