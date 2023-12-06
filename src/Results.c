#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 1000
#define DELIMITER ","

void findText(const char *filename, const char *search_term) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Kjellu's fil ku' ikke åbne.\n");
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

int main() {
    char filename[] = "Uni.csv";
    char search_term[MAX_LEN];

    scanf("%s", search_term);

    findText(filename, search_term);

    return 0;
}
