#include <limits.h>



#define MAX_FILES 100

typedef struct {
    int number;
    char name[PATH_MAX];
} file_names;


void Load_profile();

int directory_exists(const char *path);
void scan_file_names(const char *dir_path, file_names *files, int *file_count);