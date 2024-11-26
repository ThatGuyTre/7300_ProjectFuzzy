#include "sentence_search.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Function to count the number of fuzzy matches for a search term in a sentence
int sentenceSearch(const char *sentence, const char *searchTerm, int (*fuzzyFunction)(const char *, const char *)) {
    char *sentenceCopy = strdup(sentence);
    if (sentenceCopy == NULL) {
        perror("Failed to duplicate sentence");
        return -1;
    }

    const char *delimiters = " .,!?;:()\""; // Delimiters for tokenization
    int hitCount = 0;

    // Tokenize the sentence using specified delimiters
    char *token = strtok(sentenceCopy, delimiters);
    while (token != NULL) {
        // Use the passed fuzzy function to compare the search term with the word
        int threshold = 1; // Threshold for distance-based match
        if (fuzzyFunction == bruteForceFuzzy) {
            threshold = 0;
        }
        int result = fuzzyFunction(searchTerm, token);
        if (result <= threshold && result >= 0) { // Do not count -1
            // printf("Match found: %s and %s\n", searchTerm, token);
            hitCount++;
        }
        token = strtok(NULL, delimiters);
    }

    free(sentenceCopy); // Free the copied sentence
    return hitCount;
}
