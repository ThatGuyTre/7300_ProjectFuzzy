#ifndef FUZZY_H
#define FUZZY_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

char* custom_strdup(const char* s);

// Brute-force fuzzy search
int bruteForceFuzzy(const char *pattern, const char *text);

// Hamming distance-based fuzzy search
int hammingFuzzy(const char *pattern, const char *text);

// Levenshtein distance-based fuzzy search
int levenFuzzy(const char *pattern, const char *text);

void cleanupDpArray();

#endif
