// Grade_Sheet.h

#ifndef GRADE_SHEET_H
#define GRADE_SHEET_H

#include <stdbool.h>

#define MAX_COURSES 8
#define MAX_LEVELS 3
#define MAX_GRADE 10
#define MAX_CPR_LENGTH 10
#define MAX_CHARACTERS 100

typedef struct {
    char course[MAX_COURSES][MAX_CHARACTERS];
    int grade[MAX_COURSES];
    char level[MAX_COURSES][MAX_CHARACTERS];
} GradeSheet;

typedef struct {
    char cpr[MAX_CPR_LENGTH];
    GradeSheet gradeSheet;
} UserGradeData;

void compareGrades(UserGradeData userGradeData, char *eduDataPath);

#endif // GRADE_SHEET_H
