#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Results.h"


void Display_results() {
    char filename[] = "Uni.csv";
    char search_term[MAX_LEN];

    printf("Indtast term \n");
    scanf("%s", search_term);

    return;
}

void findText(const char *filename, const char *search_term) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Kjellu's csv-fil ku' ikke åbne.\n");
        return;
    }

    char line[MAX_LEN];
    while (fgets(line, sizeof(line), file)) {
        char *token = strtok(line, DELIMITER);
        if (token != NULL && strcmp(token, search_term) == 0) {
            char *text = strtok(NULL, "\n");
            if (text != NULL) {
                printf("%s, %s\n", search_term, text);
                fclose(file);
                return;
            }
        }
    }

    printf("Ingen tekst fundet for '%s'.\n", search_term);
    fclose(file);
}