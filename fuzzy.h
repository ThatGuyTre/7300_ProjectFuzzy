#ifndef FUZZY_H
#define FUZZY_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

// Generate cyclic permutations of a pattern
char** generateCyclicPermutations(const char* pattern, int* count);

// Brute-force fuzzy search
int bruteForceFuzzy(const char *pattern, const char *text);

// Brute-force cyclic
int bruteForceCyclicFuzzy(const char *pattern, const char *text);

// Hamming distance-based fuzzy search
int hammingFuzzy(const char *pattern, const char *text);

// Levenshtein distance-based fuzzy search
int levenFuzzy(const char *pattern, const char *text);

void cleanupDpArray();

#endif
