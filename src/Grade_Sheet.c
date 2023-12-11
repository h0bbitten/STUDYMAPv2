// Grade_Sheet.c

#include "Grade_Sheet.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void compareGrades(UserGradeData userGradeData, char *eduDataPath) {
    FILE *eduDataFile = fopen(eduDataPath, "r");

    if (eduDataFile == NULL) {
        perror("Error opening education data file");
        return;
    }

    char line[MAX_CHARACTERS];
    char *token;

    // Read and ignore the first line (header) from the education data file
    fgets(line, sizeof(line), eduDataFile);

    // Read each line from the education data file
    while (fgets(line, sizeof(line), eduDataFile) != NULL) {
        // Initialize GradeSheet structure for the current line
        GradeSheet eduGradeSheet;

        // Tokenize the line using ',' as a delimiter
        token = strtok(line, ",");
        strcpy(eduGradeSheet.course[0], token);

        for (int i = 0; i < MAX_COURSES; ++i) {
            token = strtok(NULL, ",");
            if (token != NULL) {
                strcpy(eduGradeSheet.course[i], token);

                token = strtok(NULL, ",");
                eduGradeSheet.grade[i] = atoi(token);

                token = strtok(NULL, ",");
                strcpy(eduGradeSheet.level[i], token);
            }
        }

        // Perform grade comparisons
        for (int i = 0; i < MAX_COURSES; ++i) {
            for (int j = 0; j < MAX_COURSES; ++j) {
                if (strcmp(userGradeData.gradeSheet.course[i], eduGradeSheet.course[j]) == 0 &&
                    userGradeData.gradeSheet.grade[i] < eduGradeSheet.grade[j] &&
                    strcmp(userGradeData.gradeSheet.level[i], eduGradeSheet.level[j]) == 0) {
                    printf("CPR %s has a higher grade in %s at level %s.\n",
                           userGradeData.cpr, eduGradeSheet.course[j], eduGradeSheet.level[j]);
                }
            }
        }
    }

    fclose(eduDataFile);
}
