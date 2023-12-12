#include "Registration.h"
#include "Main_menu.h"
#include "Questionnaire.h"
#include "KNN.h"
#include "Results.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>

void End_menu(bool *exit_program, bool *log_out);

int main() {
    const char *folderPath = "./Databases/Edu_data"; // Adjust this to fit the Edu_data path if it changes :)

    // Count the files
    int fileCount = countFilesInFolder(folderPath);

    if (fileCount != -1) {
        printf("Number of files in folder: %d\n", fileCount);

        // Declare an array of results
        numResults resultArray[fileCount];


        // Use the results as needed
        // ...
    } else {
        printf("Failed to count files in the folder.\n");
    }


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

/*

TODO
 Filter results!!!
 Do read_results                                      -       done
 Do read_edu                                          -       done
 Do print_results                                     -       done
 Calculate match compatibility                        -       done
 Change options for new test (not "666")              -       done
 Loop for whole program                               -       done
 Options after displaying result (logout, new test, delete result, exit program, show more results, choose other save)      -       done
 Write description for each education - Veiz
 Add more educations? - Allan
 Make results dynamic - not static using magic numbers - Frejerik
 Make functions same syntax, "example_function", not "ExampleFunction" - Allan
 Fix user experience, what's written out to the user - Allan
 Clean up code, remove unused stuff, don't break the program - Jonas
 IDK, something else - SØren, Thomas
 */

void End_menu(bool *exit_program, bool *log_out){


    printf("\nlogout (l), delete result (d), exit program (q), new test, show more results, choose other save, back to main menu (m)\n");

    printf("\nChoose an option for what to do now\n");

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
}





