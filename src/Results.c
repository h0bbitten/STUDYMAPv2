#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Results.h"

void Display_results() {
    char filename[] = "Uni.csv";
    char search_term[MAX_LEN];

    printf("Enter search term: \n");
    scanf("%s", search_term);

    // Call findText function with filename and search_term
    findText(filename, search_term);
}

void findText(const char *filename, const char *search_term) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Could not open %s.\n", filename);
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

    printf("No text found for '%s'.\n", search_term);
    fclose(file);
}
