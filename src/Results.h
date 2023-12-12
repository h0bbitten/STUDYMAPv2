#ifndef RESULTS_H
#define RESULTS_H

#define MAX_LEN 1000
#define DELIMITER ","

typedef struct {
    int resultValue;
} numResults;

typedef struct {
    char* name;
    double value;

} results;

typedef struct {
    char* name;
    char* link;
    char* description;

} educations;

static int numFiles = 0;

void Display_results();

void read_results(char* file_path, results result[numFiles]);
void read_edu_data(char* file_path, educations education[numFiles]);
void filter_results(results* result, educations* education, int numFiles);
void print_results(results result[numFiles], educations education[numFiles], int num_to_print);
void findText(const char *filename, const char *search_term);
int countFilesInFolder(const char *folderPath);

#endif /* RESULTS_H */
