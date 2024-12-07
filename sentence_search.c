#include "sentence_search.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Function to count the number of fuzzy matches for a search term in a sentence
int sentenceSearch(const char *sentence, const char *searchTerm, int threshold, int (*fuzzyFunction)(const char *, const char *)) {

    int hitCount = 0;
    if (fuzzyFunction == bruteForceFuzzy) {
        threshold = 0;
    }

    int result = fuzzyFunction(searchTerm, sentence);
    if (result <= threshold && result >= 0) { // don't count -1
       //printf("Match found: %s and %s\n", searchTerm, sentence);
        hitCount++;
    }

    return hitCount;
}
