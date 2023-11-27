#include "data_collection.h"
#include "Grade Sheet.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int main() {
    // Sample data - replace this with your own student information
    const char *students[] = {"A", "B", "C"};
    int CPR[] = {1,2,3};
    int math_grades[] = {02, 02, 02};
    int english_grades[] = {12, 12, 12};
    int science_grades[] = {07, 07, 07};
    int num_students = sizeof(students) / sizeof(students[0]);

    // Specify the file name
    const char *csv_file_name = "grade_sheet.csv";

    // Open the file for writing
    FILE *file = fopen(csv_file_name, "w");

    // Check if the file opened successfully
    if (file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", csv_file_name);
        return 1;
    }

    // Write the header
    fprintf(file, "CPR,Name,Math Grade,English Grade,Science Grade\n");

    // Write the student data
    for (int i = 0; i < num_students; ++i) {
        fprintf(file, "%d,%s,%d,%d,%d\n", CPR[i], students[i], math_grades[i], english_grades[i], science_grades[i]);
    }

    // Close the file
    fclose(file);

    return 0;
}