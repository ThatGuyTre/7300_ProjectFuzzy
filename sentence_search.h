#ifndef SENTENCE_SEARCH_H
#define SENTENCE_SEARCH_H

#include "fuzzy.h"

// Function to count the number of fuzzy matches for a search term in a sentence
int sentenceSearch(const char *sentence, const char *searchTerm, int (*fuzzyFunction)(const char *, const char *));

#endif
