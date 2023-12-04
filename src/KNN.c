#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

//ref devcoons, maybe?
#define DATATYPE double

struct Sample {
    DATATYPE *dimensions;
    uint32_t group;
    DATATYPE tmp_distance;
};

struct KnnData {
    uint32_t k;
    struct Sample **best_voters;
    struct Sample *samples[2];
    uint32_t samples_count[2];
    uint32_t samples_dimensions[9];
};
// Function to parse a string into a sample structure
void parseStringToSample(struct Sample *sample, char *string, uint32_t max_dimensions, uint8_t has_group);

void parseStringToSample(struct Sample *sample, char *string, uint32_t max_dimensions, uint8_t has_group);
void parseFileToSamples(struct KnnData *knn, char *filepath);
void parseSamplesToFile(struct KnnData *knn, char *filepath);
void knnAlgorithm(struct KnnData *knn);
void sortAscVoters(struct KnnData *knn);

int knn() {
    struct KnnData knn;

    // Set number of voters (k = 3)
    knn.k = 3;

    // Allocate memory for best voters
    knn.best_voters = (struct Sample **) malloc(knn.k * sizeof(struct Sample *));

    // Parse training samples file (Answers.csv)
    parseFileToSamples(&knn, "Answers.csv");

    // Parse uncategorized samples file (datast.csv)
    parseFileToSamples(&knn, "datast.csv");

    // Perform k-NN algorithm
    printf("Perform k-NN algorithm\n");
    knnAlgorithm(&knn);

    // Display "Your answers" (previously "Training Samples")
    printf("Your Answers:\n");
    for (int i = 0; i < knn.samples_count[0]; i++) {
        printf("Answer %d -", i + 1);
        for (int j = 0; j < knn.samples_dimensions[0]; j++)
            printf(" %f", (knn.samples[0] + i)->dimensions[j]);
        printf(" | %d\n", (knn.samples[0] + i)->group);
    }

    // Display "Recommended Educations" (previously "New Categorized Samples")
    printf("Recommended Educations:\n");
    for (int i = 0; i < knn.samples_count[1]; i++) {
        printf("Education %d -", i + 1);
        for (int j = 0; j < knn.samples_dimensions[1]; j++)
            printf(" %f", (knn.samples[1] + i)->dimensions[j]);
        printf(" | %d\n", (knn.samples[1] + i)->group);
    }

    // Free allocated memory
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < knn.samples_count[i]; j++) {
            if ((knn.samples[i] + j)->dimensions != NULL) {
                free((knn.samples[i] + j)->dimensions);
                (knn.samples[i] + j)->dimensions = NULL;
            }
        }
        knn.samples[i] = NULL;
    }

    free(knn.best_voters);

    return 0;
}


void parseStringToSample(struct Sample *sample, char *string, uint32_t max_dimensions, uint8_t has_group) {
    int tmp_count = has_group == 0 ? 0 : 1;
    char *tmp_ptr = strtok(string, ",");

    // Allocate memory for dimensions
    sample->dimensions = (DATATYPE *)malloc(max_dimensions * sizeof(DATATYPE));
    if (sample->dimensions == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    if (has_group == 0)
        sample->group = atoi(tmp_ptr);
    else
        sample->dimensions[0] = atof(tmp_ptr);

    while ((tmp_ptr = strtok(NULL, ",")) != NULL)
        sample->dimensions[tmp_count++] = atof(tmp_ptr);
}

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
    fgets(line, 256, file_ptr);

    tmp_count = strstr(line, "uncategorized") == NULL ? 0 : 1;

    // Read samples count, dimensions, and allocate memory
    fgets(line, 256, file_ptr);
    knn->samples_count[tmp_count] = atoi(line);

    fgets(line, 256, file_ptr);
    knn->samples_dimensions[tmp_count] = atoi(line);

    // Read each line in the file and parse it into samples
    for (uint32_t i = 0; i < knn->samples_count[tmp_count]; i++) {
        (knn->samples[tmp_count] + i)->dimensions =
                (DATATYPE *)malloc(knn->samples_dimensions[tmp_count] * sizeof(DATATYPE));

        fgets(line, 256, file_ptr);
        parseStringToSample(knn->samples[tmp_count] + i, line, knn->samples_dimensions[tmp_count], tmp_count);
    }

    fclose(file_ptr);
}

void parseSamplesToFile(struct KnnData *knn, char *filepath) {
    FILE *file_ptr = fopen(filepath, "w");

    // Check if the file was opened successfully
    if (file_ptr == NULL) {
        fprintf(stderr, "Error opening file: %s\n", filepath);
        exit(EXIT_FAILURE);
    }

    // Write the number of samples
    fprintf(file_ptr, "Categorized,%d,%d\n", knn->samples_count[1], knn->samples_dimensions[1]);

    // Write each sample to the file
    for (int i = 0; i < knn->samples_count[1]; i++) {
        for (int j = 0; j < knn->samples_dimensions[1]; j++) {
            fprintf(file_ptr, "%f,", (knn->samples[1] + i)->dimensions[j]);
        }
        fprintf(file_ptr, "%d\n", (knn->samples[1] + i)->group);
    }

    fclose(file_ptr);
}

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

            if (*(most_common[1] + j) > *(most_common[1] + selected_group_pos))
                selected_group_pos = j;
        }

        // Assign the most common group to the uncategorized sample
        (knn->samples[1] + i)->group = *(most_common[0] + selected_group_pos);

        // Print the result for the uncategorized sample
        printf("Uncategorized Sample %d - Predicted Group: %d\n", i, (knn->samples[1] + i)->group);

        // Print the 3 nearest educations
        printf("3 Nearest Educations:\n");
        for (int j = 0; j < 3 && knn->best_voters[j] != NULL; j++) {
            printf("Education %d - Group: %d - Dimensions:", j + 1, knn->best_voters[j]->group);
            for (int q = 0; q < knn->samples_dimensions[0]; q++) {
                printf(" %f", knn->best_voters[j]->dimensions[q]);
            }
            printf(" - Distance: %f\n", knn->best_voters[j]->tmp_distance);
        }
        printf("\n");
    }

    // Free allocated memory
    free(most_common[0]);
    free(most_common[1]);
}
