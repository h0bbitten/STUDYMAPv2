#ifndef RESULTS_H
#define RESULTS_H

#define MAX_LEN 1000
#define DELIMITER ","


typedef struct {
    char* name;
    double value;

} results;

typedef struct {
    char* name;
    char* link;
    char* description;

} educations;

void Display_results();

void read_results(char* file_path, results result[8]);
void read_edu_data(char* file_path, educations education[8]);
void filter_results(results result[8]);
void print_results(results result[8], educations education[8], int num_to_print);

#endif /* RESULTS_H */
