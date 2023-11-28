#include <stdbool.h>
#include <stdio.h>

#define MAX_CHARACTERS 100
#define LINE_LENGTH 256


void Questionnaire();
void questions(char* last_question_id);
void question(char *prompt, char *question_id);
int read_only_integer(bool *valid_question);
void clear_terminal();

bool check_existing_completed(FILE *file, int *line_number);
char* get_last_question_id(FILE *file, char *last_question_ID, int line_number);
void update_user_answers(FILE *file, char* last_question_ID, int line_number);


void get_date(char *formattedDate);