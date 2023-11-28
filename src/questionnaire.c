#include "questionnaire.h"
#include "data_collection.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


void Questionnaire(){
    //Check if questionaire file exists, if not creates one

    FILE *Answers;

    Answers = fopen("Answers.csv", "r");
    if(Answers == NULL)
    {
        Answers = fopen("Answers.csv", "w");
    }
    fclose(Answers);

    //Looks for existing questionaire for user and checks if they are completed or in progress.
    Answers = fopen("Answers.csv", "r");
    bool in_progress = check_existing_completed(Answers);
    fclose(Answers);
    //Checks if there is a questionnaire in progress
    //If it isn't then do the questionnaire normally/from the start
    if (in_progress == false){
        printf("\nCreating new questionnaire\n");
        // Creates a space in the answer file for current user
        Answers = fopen("Answers.csv", "a");
        fprintf(Answers, "\n%s,", current_user.username);
        //Closes the questionaire file
        fclose(Answers);

        questions("q_00");

        // Marks completed questionaire and goes to the next line
        Answers = fopen("Answers.csv", "a");
        fprintf(Answers, "done,");
        //Closes the questionnaire file
        fclose(Answers);
    }
    //If it is in progress then do the questionnaire from where the user left of
    else if (in_progress == true) {
        printf("\nContinuing previous questionnaire\n");
        // Get the id of the last question answered
        Answers = fopen("Answers.csv", "r");
        char last_question_ID[LINE_LENGTH];
        char* last_question_id_ptr = get_last_question_id(Answers, last_question_ID);
        fclose(Answers);
        printf("\nLast question ID: %s \n", last_question_ID);
        Answers = fopen("Answers.csv", "r+");
        update_user_answers(Answers, last_question_id_ptr);
        fclose(Answers);
    }
}

void questions(char* last_question_id){
    //start of the questionaire
    //question 1
    if (strcmp(last_question_id, "q_00") == 0) {
        question("Practical learning is important to me", "q_01");
        last_question_id = "q_01";
    }

    //question 2
    if (strcmp(last_question_id, "q_01") == 0) {
        question("Programming is interesting to me", "q_02");
        last_question_id = "q_02";
    }

    //question 3
    if (strcmp(last_question_id, "q_02") == 0) {
        question("I'm interested in working with software", "q_03");
        last_question_id = "q_03";
    }
    //question 4
    if (strcmp(last_question_id, "q_03") == 0) {
        question("Studying in groups is important to me", "q_04");
        last_question_id = "q_04";
    }
    //question 5
    if (strcmp(last_question_id, "q_04") == 0) {
        question("I prefer physical attendance over self-study", "q_05");
        last_question_id = "q_05";
    }
    //question 6
    if (strcmp(last_question_id, "q_05") == 0) {
        question("I enjoy studying and working with math", "q_06");
        last_question_id = "q_06";
    }
    //question 7
    if (strcmp(last_question_id, "q_06") == 0) {
        question("Work projects are important to me", "q_07");
        last_question_id = "q_07";
    }
    //question 8
    if (strcmp(last_question_id, "q_07") == 0) {
        question("I prefer structured schedules over loose schedules", "q_08");
        last_question_id = "q_08";
    }
    //question 9
    if (strcmp(last_question_id, "q_08") == 0) {
        question("I want to gain access to a wide variety of carrier paths", "q_09");
        last_question_id = "q_09";
    }
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

bool check_existing_completed(FILE *file) {
    char buffer[LINE_LENGTH];

    // Reads each line into buffer and checks if the first cell is equal to "username"
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        // Create a separate buffer for strtok
        char line[LINE_LENGTH];
        strcpy(line, buffer);

        char *token = strtok(line, ",");
        if (token != NULL && strcmp(token, current_user.username) == 0) {
            // Found the previous questionnaire, now check if the last cell is equal to "done"
            token = strtok(NULL, ",");
            while (token != NULL) {
                if (strcmp(token, "done") == 0) {
                    return false; // User exists and questionnaire is completed
                }
                token = strtok(NULL, ",");
            }
            return true; // User exists, but questionnaire is not completed
        }
    }
    return false; // User does not exist
}

char* get_last_question_id(FILE *file, char *last_question_ID) {
    char buffer[LINE_LENGTH];

    // Initialize last_question_ID to an empty string
    last_question_ID[0] = '\0';

    // Read each line into buffer, by ',' and checks if the first cell is equal to "username"
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        // Remove the newline character if it exists
        size_t length = strlen(buffer);
        if (length > 0 && buffer[length - 1] == '\n') {
            buffer[length - 1] = '\0';
        }

        // Create a separate buffer for strtok
        char line[LINE_LENGTH];
        strcpy(line, buffer);

        char *token = strtok(line, ",");
        if (token != NULL && strcmp(token, current_user.username) == 0) {
            // Traverse the tokens to find the second to last element
            char *last_token = NULL;
            char *second_last_token = NULL;
            while (token != NULL) {
                second_last_token = last_token;
                last_token = token;
                token = strtok(NULL, ",");
            }

            // If there is a second to last element, copy it to last_question_ID
            if (second_last_token != NULL) {
                strncpy(last_question_ID, second_last_token, LINE_LENGTH);
                last_question_ID[LINE_LENGTH - 1] = '\0'; // Ensure null-termination
            }
            return last_question_ID; // Exit the loop after finding the first match
        }
    }
}

void update_user_answers(FILE *file, char* last_question_ID) {
    char buffer[LINE_LENGTH];
    char user_line[LINE_LENGTH];
    user_line[0] = '\0';  // Initialize to an empty string
    FILE *tempFile;
    tempFile = fopen("temp.csv", "w");

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        char line[LINE_LENGTH];
        strcpy(line, buffer);
        char *token = strtok(line, ",");
        if (token != NULL && strcmp(token, current_user.username) == 0) {
            strcat(user_line, buffer);
            fprintf(tempFile, "%s", user_line);
            questions(last_question_ID);
            // Find the position of newline character in the line
            char *newline_position = strchr(line, '\n');
            // If newline character is found, replace it with a comma and append new data
            if (newline_position != NULL) {
                *newline_position = '\0';}
        } else {
            // If the line doesn't contain the current username, copy it as it is
            fprintf(tempFile, "%s", buffer);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove("Answers.csv");
    rename("temp.csv", "Answers.csv");
}


/*
void update_user_answers(FILE *file, char* last_question_ID) {
    char buffer[LINE_LENGTH];
    char user_line[LINE_LENGTH];
    user_line[0] = '\0';  // Initialize to an empty string

    // Length of the username
    size_t username_length = strlen(current_user.username);

    // Iterate through the file to find the line with the username
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        // Check if the line contains the current username followed by a comma
        if (strncmp(buffer, current_user.username, username_length) == 0 &&
            buffer[username_length] == ',') {
            // Concatenate the entire line to the user_line buffer
            strcat(user_line, buffer);
            break;  // Exit the loop after finding the line
        }
    }

    // Now, the user_line buffer contains the entire line with the username
    printf("User Line: %s\n", user_line);

    // You can use user_line as needed, for example, appending last_question_ID
    // ...

    // You can also rewind the file pointer if you need to read the file from the beginning
    // rewind(file);
}*/
