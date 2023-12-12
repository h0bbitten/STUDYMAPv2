#include "Questionnaire.h"
#include "Registration.h"
#include "Main_menu.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <limits.h>

char* dir_answers_path;
char the_time[40];

void Questionnaire(){

    //Create directory for answers
    make_directory("Databases/Answers");

    //Create directory for answers for current users
    make_directory(dir_answers_path);

    //Check if questionnaire file exists, if not creates one
    FILE *Answers;

    Answers = fopen(answers_path, "r");
    if(Answers == NULL)
    {
        Answers = fopen(answers_path, "w");
    }
    fclose(Answers);

    //Looks for existing questionnaire for user and checks if they are completed or in progress.
    bool in_progress = check_in_progress(answers_path);

    //Checks if there is a questionnaire in progress
    //If it isn't then do the questionnaire normally/from the start
    if (in_progress == false){
        printf("\nCreating new questionnaire\n");

        questions("q_00");

        // Marks completed questionaire
        Answers = fopen(answers_path, "a");
        fprintf(Answers, "done");
        //Closes the questionnaire file
        fclose(Answers);
    }
    //If it is in progress then do the questionnaire from where the user left of
    else if (in_progress == true) {
        printf("\nUnfinished test found!\nContinuing previous questionnaire\n");

        //Update name/date of file
        char* new_name = (char*)malloc(PATH_MAX);
        if (!new_name) {
            fprintf(stderr, "Error allocating memory for new_name.\n");
        }
        snprintf(new_name, PATH_MAX, "%s/%s.csv", dir_answers_path, the_time);
        rename(answers_path, new_name);
        strcpy(answers_path,new_name);

        // Get the id of the last question answered
        char* last_question_id = get_last_question_id(answers_path);

        //Continue where the questionnaire left off
        questions(last_question_id);

        // Marks completed questionaire
        Answers = fopen(answers_path, "a");
        fprintf(Answers, "done");
        //Closes the questionnaire file
        fclose(Answers);
    }
}

void questions(char* last_question_id){
    //start of the questionaire
    //question 1
    if (strcmp(last_question_id, "q_00") == 0) {
        question("I value hands-on learning experiences", "q_01");
        last_question_id = "q_01";
    }
    //question 2
    if (strcmp(last_question_id, "q_01") == 0) {
        question("Programming is interesting to me", "q_02");
        last_question_id = "q_02";
    }
    //question 3
    if (strcmp(last_question_id, "q_02") == 0) {
        question("I have a keen interest in working with software development", "q_03");
        last_question_id = "q_03";
    }
    //question 4
    if (strcmp(last_question_id, "q_03") == 0) {
        question("I prefer attending classes in person rather than self-study", "q_04");
        last_question_id = "q_04";
    }
    //question 5
    if (strcmp(last_question_id, "q_04") == 0) {
        question("I find joy in studying and applying mathematical concepts", "q_05");
        last_question_id = "q_05";
    }
    //question 6
    if (strcmp(last_question_id, "q_05") == 0) {
        question("Collaborating with others in study groups is important to me", "q_06");
        last_question_id = "q_06";
    }
    //question 7
    if (strcmp(last_question_id, "q_06") == 0) {
        question("Work projects are important to me", "q_07");
        last_question_id = "q_07";
    }
    //question 8
    if (strcmp(last_question_id, "q_07") == 0) {
        question("I aspire to gain access to a diverse range of career paths", "q_08");
        last_question_id = "q_08";
    }
    //question 9
    if (strcmp(last_question_id, "q_08") == 0) {
        question("I prefer well-defined assignments and tasks", "q_09");
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

bool check_in_progress(char *file_path) {

    FILE *file = fopen(file_path, "r");

    if (file == NULL) {
        fprintf(stderr,"Error opening file: %s", file_path);
        return false;
    }

    // Check if the file is empty
    if (fgetc(file) == EOF) {
        fclose(file);

        // Delete the file if it's empty
        if (remove(file_path) != 0) {
            fprintf(stderr, "Error deleting empty file");
        }

        return false;
    }

    // Reset file position indicator to the beginning
    rewind(file);

    char line[1024];  // Assuming a maximum line length of 1024 characters, adjust as needed

    while (fgets(line, sizeof(line), file) != NULL) {
        // Remove newline character if present
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        // Find the last cell in the line
        char *last_cell = strrchr(line, ',');

        // Check if the last cell exists and equals "done"
        if (last_cell != NULL && strcmp(last_cell + 1, "done") == 0) {
            fclose(file);
            return false;
        }
    }
    fclose(file);

    return true;
}

char* get_last_question_id(char *filename) {
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }

    char line[1024];
    char *second_last_cell = NULL;

    while (fgets(line, sizeof(line), file) != NULL) {
        // Remove newline character if present
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        // Tokenize the line to extract cells
        char *token = strtok(line, ",");
        char *last_cell = NULL;
        char *second_to_last_cell_temp = NULL;

        while (token != NULL) {
            second_to_last_cell_temp = last_cell;
            last_cell = token;
            token = strtok(NULL, ",");
        }

        // Check if the second-to-last cell exists
        if (second_to_last_cell_temp != NULL) {
            // Allocate memory for the result and copy the content
            second_last_cell = strdup(second_to_last_cell_temp);
            break;
        }
    }

    fclose(file);
    return second_last_cell;
}


void get_date(char *formatted_date) {
    // Get the current time
    time_t current_time;
    time(&current_time);

    // Convert the current time to a structure representing local time
    struct tm *local_time = localtime(&current_time);

    // Format the date as "SS-MM-HH-dd-mm-yyyy" and store it in a string
    strftime(formatted_date, 40, "%Y-%m-%d-%H-%M-%S", local_time);

}