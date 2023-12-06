#include "load_profile.h"
#include "data_collection.h"
#include "questionnaire.h"

#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#define MAX_FILES 100

typedef struct {
    int number;
    char name[PATH_MAX];
} NumberedFile;


#include <sys/stat.h>
#ifdef _WIN32
#include <direct.h>
#define mkdir(path, mode) _mkdir(path)
#endif

//char *dir_answers_path;


int directory_exists(const char *path) {
    struct stat info;

    if (stat(path, &info) != 0) {
        // Error accessing file/directory
        //fprintf(stderr, "Error accessing file/directory for: %s", path);
        return 0;
    } else if (info.st_mode & S_IFDIR) {
        // It's a directory
        return 1;
    } else {
        // It's not a directory
        return 0;
    }
}

void scan_file_names(const char *dir_path, NumberedFile *files, int *file_count) {
    DIR *dir = opendir(dir_path);

    if (dir == NULL) {
        perror("Error opening directory");
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
            // You can add more conditions for other types like directories, symbolic links, etc.
        }
    }

    closedir(dir);
}

void Load_profile(){

    //Gets the date and time for the start of the questionnaire
    get_date(the_time);

    //Create path for directory for answers for current user
    dir_answers_path = (char*)malloc(PATH_MAX);
    if (!dir_answers_path) {
        fprintf(stderr, "Error allocating memory for dir_results_path.\n");
    }
    snprintf(dir_answers_path, PATH_MAX, "Databases/Answers/%s", current_user.username);


    //Check if the directory already exists
    if (directory_exists(dir_answers_path)) {
        printf("Directory exists!!!!!!!.\n");

        // Array to store numbered files
        NumberedFile files[MAX_FILES];
        int file_count = 0;

        // Scan file names and assign a number to each file
        scan_file_names(dir_answers_path, files, &file_count);

        printf("Previously saved answers:\n");
        // Display all the files in the directory
        for (int i = 0; i < file_count; i++) {
            printf("%d: Answers given on %s\n", files[i].number, files[i].name);
        }

        printf("\nChoose a save to use or take a new test (666)\n>");
        bool valid_input = false;
        int file_number;
        do {
            file_number = read_only_integer(&valid_input);
        } while (!valid_input || file_number <= 0 || file_number > file_count && file_number != 666);

        //Create path for file for answers for current user
        answers_path = (char*)malloc(PATH_MAX);
        if (!answers_path) {
            fprintf(stderr, "Error allocating memory for answers_path.\n");
        }

        if (file_number == 666){
            snprintf(answers_path, PATH_MAX, "%s/%s.csv", dir_answers_path, the_time);
        }
        else {
            snprintf(answers_path, PATH_MAX, "%s/%s.csv", dir_answers_path, files[file_number - 1].name);
        }
    }
    else {
        printf("Directory does not exist!!!!!!!!!!.\n");

        //Create path for file for answers for current user
        answers_path = (char*)malloc(PATH_MAX);
        if (!answers_path) {
            fprintf(stderr, "Error allocating memory for answers_path.\n");
        }
        snprintf(answers_path, PATH_MAX, "%s/%s.csv", dir_answers_path, the_time);

    }



}