#include "Registration.h"
#include "Main_menu.h"
#include "Questionnaire.h"
#include "KNN.h"
#include "Results.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>

void amount_file();
void End_menu(bool *exit_program, bool *log_out);

int main() {

    amount_file();

    bool exit_program = false;
    bool log_out = true;
    bool do_questionnaire = true;

    do {

        if (log_out == true){
            Registration();
        }

        Main_menu(&do_questionnaire);

        if(do_questionnaire == true){
            Questionnaire();
            KNN();
        }

        Display_results();

        End_menu(&exit_program, &log_out);

    } while (exit_program == false);

return 0;
}

void End_menu(bool *exit_program, bool *log_out){


    printf("\n* Exit program (press 'Q')\n"
                    "* Back to main menu (press 'M')\n"
                    "* Logout (press 'L')\n"
                    "* Delete result (press 'D')\n");

    printf("\nPlease choose an action above\n>");

    char* input = NULL;

    do {

        input = read_input();

    } while ((input == NULL || strcmp(input, "d") != 0) && (input == NULL || strcmp(input, "D") != 0) &&
            (input == NULL || strcmp(input, "m") != 0) && (input == NULL || strcmp(input, "M") != 0) &&
            (input == NULL || strcmp(input, "l") != 0) && (input == NULL || strcmp(input, "L") != 0) &&
            (input == NULL || strcmp(input, "q") != 0) && (input == NULL || strcmp(input, "Q") != 0));


    if (strcmp(input, "d") == 0 || strcmp(input, "D") == 0){
        if (remove(result_path) != 0) {
            fprintf(stderr,"Error deleting file");
        }
        *exit_program = false;
        *log_out = false;
    }
    else if (strcmp(input, "l") == 0 || strcmp(input, "L") == 0){
        *exit_program = false;
        *log_out = true;
    }
    else if (strcmp(input, "m") == 0 || strcmp(input, "M") == 0){
        *exit_program = false;
        *log_out = false;
    }
    else if (strcmp(input, "q") == 0 || strcmp(input, "Q") == 0){
        *exit_program = true;
    }
    clear_terminal();
}


void amount_file(){
    const char *folderPath = "./Databases/Edu_data"; // Adjust this to fit the Edu_data path if it changes :)


    // Count the files
    int fileCount = countFilesInFolder(folderPath);

    if (fileCount != -1) {
        // Declare an array of results
        numResults resultArray[fileCount];
    } else {
        fprintf(stderr,"Failed to count files in the folder.\n");
    }
}


