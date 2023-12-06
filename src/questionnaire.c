#include "questionnaire.h"
#include "data_collection.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <limits.h>

char the_time[30];

void Questionnaire(){

    //Create path for answers for current user
    answers_path = (char*)malloc(PATH_MAX);
    if (!answers_path) {
        fprintf(stderr, "Error allocating memory for answers_path.\n");
    }
    snprintf(answers_path, PATH_MAX, "Databases/Answers/%s.csv", current_user.username);

    //Gets the date and time for the start of the questionnaire
    get_date(the_time);

    //Check if questionaire file exists, if not creates one
    FILE *Answers;

    Answers = fopen(answers_path, "r");
    if(Answers == NULL)
    {
        Answers = fopen(answers_path, "w");
    }
    fclose(Answers);

    //Looks for existing questionaire for user and checks if they are completed or in progress.
    Answers = fopen(answers_path, "r");
    int line_number = -1;
    bool in_progress = check_existing_completed(Answers, &line_number);
    fclose(Answers);
    //Checks if there is a questionnaire in progress
    //If it isn't then do the questionnaire normally/from the start
    if (in_progress == false){
        printf("\nCreating new questionnaire\n");
        // Creates a space in the answer file for current user
        Answers = fopen(answers_path, "a");
        fprintf(Answers, "%s,%s,", current_user.username, the_time);
        //Closes the questionaire file
        fclose(Answers);

        questions("q_00");

        // Marks completed questionaire
        Answers = fopen(answers_path, "a");
        fprintf(Answers, "done,\n");
        //Closes the questionnaire file
        fclose(Answers);
    }
    //If it is in progress then do the questionnaire from where the user left of
    else if (in_progress == true) {

        printf("\nUnfinished test found!\nContinuing previous questionnaire\n");

        // Get the id of the last question answered
        char last_question_ID[LINE_LENGTH];
        char* last_question_id_ptr = get_last_question_id(Answers, last_question_ID, line_number);
        fclose(Answers);

        //Continue where the questionnaire left off
        update_user_answers(Answers, last_question_id_ptr, line_number);

        // Marks completed questionaire
        Answers = fopen(answers_path, "a");
        fprintf(Answers, "done,\n");
        //Closes the questionnaire file
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
        question("I prefer physical attendance over self-study", "q_04");
        last_question_id = "q_04";
    }
    //question 5
    if (strcmp(last_question_id, "q_04") == 0) {
        question("I enjoy studying and working with math", "q_05");
        last_question_id = "q_05";
    }
    //question 6
    if (strcmp(last_question_id, "q_05") == 0) {
        question("Studying in groups is important to me", "q_06");
        last_question_id = "q_06";
    }
    //question 7
    if (strcmp(last_question_id, "q_06") == 0) {
        question("Work projects are important to me", "q_07");
        last_question_id = "q_07";
    }
    //question 8
    if (strcmp(last_question_id, "q_07") == 0) {
        question("I want to gain access to a wide variety of carrier paths", "q_08");
        last_question_id = "q_08";
    }
    //question 9
    if (strcmp(last_question_id, "q_08") == 0) {
        question("I prefer structured timelines where with informative assignments?", "q_09");
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
    FILE *Answer;
    Answer = fopen(answers_path, "a");
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

bool check_existing_completed(FILE *file, int *line_number) {
    char buffer[LINE_LENGTH];

    *line_number = 0; // Initialize line number

    // Reads each line into buffer and checks if the first cell is equal to "username"
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        (*line_number)++; // Increment line number for each line

        // Create a separate buffer for strtok
        char line[LINE_LENGTH];
        strcpy(line, buffer);

        char *token = strtok(line, ",");
        char *last_token = NULL;
        char *second_last_token = NULL;

        if (token != NULL && strcmp(token, current_user.username) == 0) {

            // Found a previous questionnaire, now check if the last cell is equal to "done"

            while (token != NULL) {
                last_token = token;

                token = strtok(NULL, ",");
                if (token != NULL){
                    second_last_token = last_token;
                    last_token = token;
                }
            }

            if (second_last_token != NULL && strcmp(second_last_token, "done") != 0) {
                return true; // User exists, and questionnaire is not completed
            }
        }
    }

    return false; // User does not exist
}

char* get_last_question_id(FILE *file, char *last_question_ID, int Line_number) {
    char buffer[LINE_LENGTH];

    file = fopen(answers_path, "r");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }

    // Initialize last_question_ID to an empty string
    last_question_ID[0] = '\0';

    int currentLineNumber = 0; // Initialize current line number

    // Read each line into buffer, ',' by ','
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        // Remove the newline character if it exists
        size_t length = strlen(buffer);
        if (length > 0 && buffer[length - 1] == '\n') {
            buffer[length - 1] = '\0';
        }

        currentLineNumber++; // Increment line number for each line

        if (currentLineNumber == Line_number) {
            // Create a separate buffer for strtok
            char line[LINE_LENGTH];
            strcpy(line, buffer);

            char *token = strtok(line, ",");
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

            fclose(file);
            if (strcmp(second_last_token, current_user.username) == 0){
                return "q_00";
            }
            return last_question_ID; // Exit the loop after finding the desired line
        }
    }

    fclose(file);
    return NULL; // Return NULL if the desired line is not found
}

void update_user_answers(FILE *file, char *last_question_ID, int line_number) {
    // Open file in read mode
    file = fopen(answers_path, "r");

    // Create temp file to store contents of previous answers
    FILE *temp_file;
    temp_file = fopen("temp.csv", "w");

    if (temp_file == NULL || file == NULL) {
        perror("Error opening file");
        return;
    }

    char line[LINE_LENGTH];
    char buffer[LINE_LENGTH];
    char output[LINE_LENGTH];
    int current_line_number = 0;

    // Copy content until the desired line
    while (fgets(line, sizeof(line), file) != NULL) {
        current_line_number++;

        // If it's the desired line, copy it to the buffer and skip writing it to the temp file
        if (current_line_number == line_number) {
            size_t line_length = strlen(line);
            if (line_length > 0 && line[line_length - 1] == '\n') {
                // Exclude the newline character
                line[line_length - 1] = '\0';
            }
            strncpy(buffer, line, LINE_LENGTH);
            buffer[LINE_LENGTH - 1] = '\0'; // Ensure null-termination
            continue;
        }

        // Otherwise, write the line to the temp file
        fputs(line, temp_file);
    }

    // If the desired line was found, copy it back to the temp file
    if (current_line_number >= line_number) {


        //Trow away the first two elements of the line
        // Use strtok to tokenize the input string
        char *token = strtok(buffer, ",");

        // Use a loop to skip the first two tokens
        for (int i = 0; i < 2 && token != NULL; ++i) {
            token = strtok(NULL, ",");
        }

        // Now 'token' contains the third element and beyond
        while (token != NULL) {
            snprintf(output, sizeof(output), "%s,%s", output, token);
            token = strtok(NULL, ",");
        }

        fprintf(temp_file, "%s,%s%s,",current_user.username, the_time, output);
    }

    fclose(file);
    fclose(temp_file);

    remove(answers_path);
    rename("temp.csv", answers_path);

    // continues questionnaire
    questions(last_question_ID);
}

void get_date(char *formattedDate) {
    // Get the current time
    time_t currentTime;
    time(&currentTime);

    // Convert the current time to a structure representing local time
    struct tm *localTime = localtime(&currentTime);

    // Format the date as "SS-MM-HH-dd-mm-yyyy" and store it in a string
    strftime(formattedDate, 20, "%S-%M-%H-%d-%m-%Y", localTime);
}