#include <limits.h>



#define MAX_FILES 100

typedef struct {
    int number;
    char name[PATH_MAX];
} NumberedFile;


void Load_profile();

int directory_exists(const char *path);
void scan_file_names(const char *dir_path, NumberedFile *files, int *file_count);