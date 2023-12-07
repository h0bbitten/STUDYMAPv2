#include <stdbool.h>
#include <stdio.h>

#define MAX_CHARACTERS 100
#define LINE_LENGTH 256

extern char *dir_answers_path;
extern char* answers_path;
extern char the_time[40];

void Questionnaire();
void questions(char* last_question_id);
void question(char *prompt, char *question_id);
int read_only_integer(bool *valid_question);
void clear_terminal();

bool check_in_progress(char *file_path);
char* get_last_question_id(char *filename);
void resume_answers(FILE *file, char *last_question_ID);


void get_date(char *formattedDate);