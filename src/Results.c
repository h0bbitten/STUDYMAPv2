#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Results.h"
#include "load_profile.h"
#include "KNN.h"

void Display_results() {
    char filename[] = "Uni.csv";
    char search_term[MAX_LEN];



    // Call findText function with filename and search_term
    findText(filename, search_term);
}


void findText(const char *filename, const char *search_term) {

    //kan filen åbne?
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Kunne ikke åbne %s.\n", filename);
        return;
    }

    //læs top svar fra result_path og find dem i uni.csv
    fprintf("%s", result_path);
    result_path = search_term;




    //få beskrivelse af uddannelser fra uni.csv udfra search_term
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

    printf("ingen tekst fundet for '%s'.\n", search_term);
    fclose(file);
}
