#include "questionnaire.h"
#include "data_collection.h"

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

    // Creates a space in the answer file for current user
    Answers = fopen("Answers.csv", "a");
    fprintf(Answers, "%s,", current_user.username);
    //Closes the questionaire file
    fclose(Answers);

    //start of the questionaire

    //question 1
    question("Practical learning is important to me", "q_01");

    //question 2
    question("Programming is interesting to me", "q_02");

    //question 3
    question("I'm interested in working with software", "q_03");

    //question 4
    question("Studying in groups is important to me", "q_04");

    //question 5
    question("I prefer physical attendance over self-study", "q_05");

    //question 6
    question("I enjoy studying and working with math", "q_06");

    //question 7
    question("Work projects are important to me", "q_07");

    //question 8
    question("I prefer structured schedules over loose schedules", "q_08");

    //question 9
    question("I want to gain access to a wide variety of carrier paths", "q_09");

    // Goes to the next line
    Answers = fopen("Answers.csv", "a");
    fprintf(Answers, "\n");
    //Closes the questionaire file
    fclose(Answers);
}

void question(char *prompt, char *question_id){
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
    fprintf(Answer, "%s,%d,", question_id, question_value);
    //Closes the questionaire file
    fclose(Answer);

    clear_terminal();
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

void clear_terminal(){
    printf("\033[2J");  // Clear the screen
    printf("\033[H");   // Move the cursor to the home position
}