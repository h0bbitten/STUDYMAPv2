#include "Main_menu.h"
#include "Registration.h"
#include "Questionnaire.h"
#include "KNN.h"

#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <string.h>
#include <ctype.h>

#include <sys/stat.h>
#ifdef _WIN32
    #include <direct.h>
    #define mkdir(path, mode) _mkdir(path)
#endif

char* dir_results_path;

void Main_menu(bool* do_questionnaire){

    //Gets the date and time for the start of the questionnaire
    get_date(the_time);

    //Create path for directory for answers for current user
    dir_answers_path = (char*)malloc(PATH_MAX);
    if (!dir_answers_path) {
        fprintf(stderr, "Error allocating memory for dir_results_path.\n");
    }
    snprintf(dir_answers_path, PATH_MAX, "Databases/Answers/%s", current_user.username);

    //Create path for directory for results for current user
    dir_results_path = (char*)malloc(PATH_MAX);
    if (!dir_results_path) {
        fprintf(stderr, "Error allocating memory for dir_results_path.\n");
    }
    snprintf(dir_results_path, PATH_MAX, "Databases/Results/%s", current_user.username);

    // Array of struct to store numbered results files
    file_names answer_files[MAX_FILES];
    int answers_file_count = 0;

    // Scan file names and assign a number to each file
    scan_file_names(dir_answers_path, answer_files, &answers_file_count);

    // Array of struct to store numbered results files
    file_names result_files[MAX_FILES];

    int print_counter = 0;

    int results_file_count = 0;


    // Scan file names and assign a number to each file
    scan_file_names(dir_results_path, result_files, &results_file_count);

    if (results_file_count > 0) {
        printf("\nPreviously saved results:\n");
        // Display all the files in the directory
        for (int i = 0; i < results_file_count; i++) {
            print_counter++;
            printf("%d: View results from test taken on %s\n", print_counter, change_date_format(result_files[i].name));
        }
    }

    int temp_results_file_count = results_file_count;
    int index = 0;

    if (answers_file_count > 0){
        for (int i = 0; i < answers_file_count; i++) {

            // Allocate memory for temp_path
            char* temp_path = (char*)malloc(PATH_MAX);
            if (temp_path == NULL) {
                fprintf(stderr, "Memory allocation failed for temp_path\n");
                break;
            }

            snprintf(temp_path, PATH_MAX, "%s/%s.csv", dir_answers_path, answer_files[i].name);
            bool in_progress = check_in_progress(temp_path);
            if (in_progress == true){
                if (temp_results_file_count == results_file_count) printf("\nTests in progress:\n");
                temp_results_file_count++;
                index++;
                answer_files[i].number = 0;
                answer_files[i].number -= index;
                print_counter++;
                printf("%d: Resume unfinished test from %s\n", print_counter,change_date_format(answer_files[i].name));
            }
            free(temp_path);
        }
    }

    if (print_counter == 0){
        //Create path for file for answers for current user
        answers_path = (char*)malloc(PATH_MAX);
        if (!answers_path) {
            fprintf(stderr, "Error allocating memory for answers_path.\n");
        }
        snprintf(answers_path, PATH_MAX, "%s/%s.csv", dir_answers_path, the_time);
        *do_questionnaire = true;

        printf("\nNo saved test found\n");
    }
    else{

        printf("\nChoose an action or take a new test (n)\n>");

        char* input = NULL;
        int number = 0;

        do {

            input = read_input();

            if (input != NULL && isdigit(*input)) {
                number = atoi(input);
            }

        } while ((input == NULL || input[0] == '\0' || (number <= 0 || number > print_counter)) && (input == NULL || strcmp(input, "n") != 0) && (input == NULL || strcmp(input, "N") != 0));


        if (strcmp(input, "n") == 0 || strcmp(input, "N") == 0){
            //Create path for file for answers for current user
            answers_path = (char*)malloc(PATH_MAX);
            if (!answers_path) {
                fprintf(stderr, "Error allocating memory for answers_path.\n");
            }
            snprintf(answers_path, PATH_MAX, "%s/%s.csv", dir_answers_path, the_time);
            *do_questionnaire = true;
        }
        else if (number <= results_file_count){
            //Create path for file for results for current user
            result_path = (char*)malloc(PATH_MAX);
            if (!result_path) {
                fprintf(stderr, "Error allocating memory for result_path.\n");
            }
            snprintf(result_path, PATH_MAX, "%s/%s.csv", dir_results_path, result_files[number - 1].name);

            *do_questionnaire = false;
        }
        else{
            int num_retrieve = (number - results_file_count);
            //Create path for file for answers for current user
            answers_path = (char*)malloc(PATH_MAX);
            if (!answers_path) {
                fprintf(stderr, "Error allocating memory for answers_path.\n");
            }
            for (int i = 0; i < answers_file_count; i++) {
                if (answer_files[i].number == -num_retrieve) {
                    snprintf(answers_path, PATH_MAX, "%s/%s.csv", dir_answers_path, answer_files[i].name);
                    *do_questionnaire = true;
                    break;
                }
            }
        }
    }
}

void scan_file_names(const char *dir_path, file_names *files, int *file_count) {
    DIR *dir = opendir(dir_path);

    if (dir == NULL) {
        fprintf(stderr, "Error opening directory %s", dir_path);
        return;
    }

    struct dirent *entry;
    struct stat file_stat;

    while ((entry = readdir(dir)) != NULL && *file_count < MAX_FILES) {
        char full_path[PATH_MAX];
        snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, entry->d_name);

        if (stat(full_path, &file_stat) == 0) {
            if (S_ISREG(file_stat.st_mode)) {
                // This is a regular file
                files[*file_count].number = *file_count + 1;

                // Use basename to get only the file name without the path or extension
                const char *base_name = basename(entry->d_name);

                // Create a writable copy of the base name
                char writable_base_name[PATH_MAX];
                strncpy(writable_base_name, base_name, sizeof(writable_base_name) - 1);
                writable_base_name[sizeof(writable_base_name) - 1] = '\0';

                // Remove the extension by finding the last '.' and replacing it with '\0'
                size_t len = strlen(writable_base_name);
                for (size_t i = len; i > 0; --i) {
                    if (writable_base_name[i - 1] == '.') {
                        writable_base_name[i - 1] = '\0';
                        break;
                    }
                }

                // Copy the modified name to the structure
                strncpy(files[*file_count].name, writable_base_name, sizeof(files[*file_count].name) - 1);
                files[*file_count].name[sizeof(files[*file_count].name) - 1] = '\0';

                (*file_count)++;
            }
        }
    }

    closedir(dir);
}

char* change_date_format(char *date_string) {

    int year, month, day, hour, minute, second;

    char *formatted_date = (char *)malloc(50);

    // Read and create readable date
    if (sscanf(date_string, "%d-%d-%d-%d-%d-%d", &year, &month, &day, &hour, &minute, &second) == 6) {
        // Format the result
        sprintf(formatted_date, "%02d:%02d, %d %s %d", hour, minute, day,
                (month == 1) ? "January" :
                (month == 2) ? "February" :
                (month == 3) ? "March" :
                (month == 4) ? "April" :
                (month == 5) ? "May" :
                (month == 6) ? "June" :
                (month == 7) ? "July" :
                (month == 8) ? "August" :
                (month == 9) ? "September" :
                (month == 10) ? "October" :
                (month == 11) ? "November" :
                (month == 12) ? "December" : "Invalid month",
                year);

        return formatted_date;
    } else {
        printf("Invalid date format\n");
        return NULL;
    }
}

char* read_input() {
    static char buffer[MAX_CHARACTERS];

    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }

        return buffer;
    }

    return NULL;
}

