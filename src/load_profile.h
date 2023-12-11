#include <limits.h>
#include <stdbool.h>


#define MAX_FILES 100

typedef struct {
    int number;
    char name[PATH_MAX];
} file_names;


void Load_profile(bool* do_questionnaire);

void scan_file_names(const char *dir_path, file_names *files, int *file_count);
char* change_date_format(char *dateString);
char* read_input();