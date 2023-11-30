#ifndef KNN_HEADER_H
#define KNN_HEADER_H

#include <inttypes.h>

#define DATATYPE double

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
    uint32_t samples_dimensions[2];
};

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

#endif //KNN_HEADER_H
