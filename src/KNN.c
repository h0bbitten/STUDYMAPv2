#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "KNN.h"

//
//You may need to generate the txt files yourself
//



#define DATATYPE double
/*
// Structure to represent a sample with its dimensions, group, and temporary distance
struct Sample {
    DATATYPE *dimensions;
    uint32_t group;
    DATATYPE tmp_distance;
};

// Structure to hold k-NN algorithm data, including k, best voters, and sample information
struct KnnData {
    uint32_t k;
    struct Sample **best_voters;
    struct Sample *samples[2];
    uint32_t samples_count[2];
    uint32_t samples_dimensions[8];
};
*/
// Function to parse a string into a sample structure
void parseStringToSample(struct Sample *sample, char *string, uint32_t max_dimensions, uint8_t has_group);

// Function to parse samples from a file into the KnnData structure
void parseFileToSamples(struct KnnData *knn, char *filepath);

// Function to parse samples from the KnnData structure into a file (placeholder)
void parseSamplesToFile(struct KnnData *knn, char *filepath);

// Function to perform the k-NN algorithm
void knnAlgorithm(struct KnnData *knn);

// Function to sort the best voters in ascending order based on temporary distances
void sortAscVoters(struct KnnData *knn);

// Main function
int knn() {
    char input_str[256];
    struct KnnData knn;

    // Input k value
    printf("Set number of voters (k=): ");
    scanf("%d", &knn.k);

    // Allocate memory for best voters
    knn.best_voters = (struct Sample **)malloc(knn.k * sizeof(struct Sample *));

    // Input and parse training samples file
    printf("Provide categorized samples file (train data): ");
    scanf("%s", input_str);
    parseFileToSamples(&knn, input_str);

    // Input and parse uncategorized samples file
    printf("Provide uncategorized samples file (new data): ");
    scanf("%s", input_str);
    parseFileToSamples(&knn, input_str);

    // Perform k-NN algorithm
    printf("Perform k-NN algorithm\n");
    knnAlgorithm(&knn);

    // Ask user if they want to save the output
    printf("Do you want to save the output (yes/no)? ");
    scanf("%s", input_str);

    // Save the output if requested
    if (strcmp(input_str, "yes") == 0 || strcmp(input_str, "y") == 0) {
        printf("Where do you want to save the newly categorized data (filepath)? ");
        scanf("%s", input_str);
        parseSamplesToFile(&knn, input_str);
        printf("Completed.\n");
    }

    // Display training samples
    printf("Training Samples:\n");
    for (int i = 0; i < knn.samples_count[0]; i++) {
        printf("Sample %d -", i);
        for (int j = 0; j < knn.samples_dimensions[0]; j++)
            printf(" %f", (knn.samples[0] + i)->dimensions[j]);
        printf(" | %d\n", (knn.samples[0] + i)->group);
    }

    // Display newly categorized samples
    printf("New Categorized Samples:\n");
    for (int i = 0; i < knn.samples_count[1]; i++) {
        printf("Sample %d -", i);
        for (int j = 0; j < knn.samples_dimensions[1]; j++)
            printf(" %f", (knn.samples[1] + i)->dimensions[j]);
        printf(" | %d\n", (knn.samples[1] + i)->group);
    }

    return 0;
}

// Function to parse a comma-separated string into a sample structure
void parseStringToSample(struct Sample *sample, char *string, uint32_t max_dimensions, uint8_t has_group) {
    int tmp_count = has_group == 0 ? 0 : 1;
    char *tmp_ptr = strtok(string, ",");

    if (has_group == 0)
        sample->group = atoi(tmp_ptr);
    else
        sample->dimensions[0] = atof(tmp_ptr);

    while ((tmp_ptr = strtok(NULL, ",")) != NULL)
        sample->dimensions[tmp_count++] = atof(tmp_ptr);
}

// Function to parse samples from a file into the KnnData structure
void parseFileToSamples(struct KnnData *knn, char *filepath) {
    int tmp_count;
    char line[256];
    FILE *file_ptr = fopen(filepath, "r");

    // Error handling for file opening
    if (file_ptr == NULL) {
        fprintf(stderr, "Error opening file: %s\n", filepath);
        exit(EXIT_FAILURE);
    }

    // Read the first line to determine the type of samples (categorized or uncategorized)
    fgets(line, 128, file_ptr);

    tmp_count = strstr(line, "uncategorized") == NULL ? 0 : 1;

    // Read samples count, dimensions, and allocate memory
    fgets(line, 128, file_ptr);
    knn->samples_count[tmp_count] = atoi(line);

    knn->samples[tmp_count] = (struct Sample *)malloc(knn->samples_count[tmp_count] * sizeof(struct Sample));

    fgets(line, 128, file_ptr);
    knn->samples_dimensions[tmp_count] = atoi(line);

    // Read each line in the file and parse it into samples
    for (uint32_t i = 0; i < knn->samples_count[tmp_count]; i++) {
        (knn->samples[tmp_count] + i)->dimensions =
                (DATATYPE *)malloc(knn->samples_dimensions[tmp_count] * sizeof(DATATYPE));

        fgets(line, 128, file_ptr);
        parseStringToSample(knn->samples[tmp_count] + i, line, knn->samples_dimensions[tmp_count], tmp_count);
    }

    fclose(file_ptr);
}

// Placeholder function to parse samples from the KnnData structure into a file
void parseSamplesToFile(struct KnnData *knn, char *filepath) {
    printf(":) not working yet..."); // Placeholder
}

// Function to sort the best voters in ascending order based on temporary distances
void sortAscVoters(struct KnnData *knn) {
    struct Sample *tmp_sample = NULL;

    for (int i = 0; i < knn->k; i++)
        for (int j = 0; j < knn->k - 1; j++)
            if (knn->best_voters[j]->tmp_distance > knn->best_voters[j + 1]->tmp_distance) {
                tmp_sample = knn->best_voters[j];
                knn->best_voters[j] = knn->best_voters[j + 1];
                knn->best_voters[j + 1] = tmp_sample;
            }
}

// Function to perform the k-NN algorithm
void knnAlgorithm(struct KnnData *knn) {
    double euclidean_distance;
    uint32_t *most_common[2], selected_group_pos;

    // Allocate memory for arrays to store most common groups and their counts
    most_common[0] = (uint32_t *)malloc(knn->k * sizeof(uint32_t));
    most_common[1] = (uint32_t *)malloc(knn->k * sizeof(uint32_t));

    // Loop through uncategorized samples
    for (int i = 0; i < knn->samples_count[1]; i++) {
        // Initialize best voters array
        for (int q = 0; q < knn->k; q++)
            knn->best_voters[q] = NULL;

        // Loop through training samples to calculate distances
        for (int j = 0; j < knn->samples_count[0]; j++) {
            euclidean_distance = 0;

            // Calculate Euclidean distance
            for (int q = 0; q < knn->samples_dimensions[0]; q++)
                euclidean_distance += pow(
                        (knn->samples[0] + j)->dimensions[q] - (knn->samples[1] + i)->dimensions[q], 2);

            (knn->samples[0] + j)->tmp_distance = sqrt(euclidean_distance);

            // Fill the initial k best voters
            if (j < knn->k) {
                knn->best_voters[j] = (knn->samples[0] + j);
            } else {
                // Sort the best voters array in ascending order based on distances
                if (j == knn->k)
                    sortAscVoters(knn);

                for (int q = 0; q < knn->k; q++)
                    if (knn->best_voters[q]->tmp_distance > (knn->samples[0] + j)->tmp_distance) {
                        for (int z = knn->k - 1; z >= q + 1; z--)
                            knn->best_voters[z] = knn->best_voters[z - 1];

                        knn->best_voters[q] = (knn->samples[0] + j);
                        break;
                    }
            }
        }

        // Initialize arrays to store most common groups and their counts
        memset(most_common[0], 0, knn->k * sizeof(uint32_t));
        memset(most_common[1], 0, knn->k * sizeof(uint32_t));

        // Count the occurrences of each group in the best voters array
        for (int j = 0; j < knn->k; j++) {
            for (int q = 0; q < knn->k; q++)
                if (*(most_common[0] + q) == knn->best_voters[j]->group) {
                    *(most_common[1] + q) += 1;
                    break;
                } else if (*(most_common[0] + q) == 0) {
                    *(most_common[0] + q) = knn->best_voters[j]->group;
                    *(most_common[1] + q) += 1;
                    break;
                }
        }

        // Find the most common group
        selected_group_pos = 0;

        for (int j = 1; j < knn->k; j++) {
            if (*(most_common[1] + j) == 0)
                break;

            if (*(most_common[0] + j) > *(most_common[0] + selected_group_pos))
                selected_group_pos = j;
        }

        // Assign the most common group to the uncategorized sample
        (knn->samples[1] + i)->group = *(most_common[0] + selected_group_pos);
    }

    // Free allocated memory
    free(most_common[0]);
    free(most_common[1]);
}
