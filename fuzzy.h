#ifndef FUZZY_H
#define FUZZY_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Placeholder for hamming distance-based fuzzy search
int hammingFuzzy(const char *pattern, const char *text);

// Placeholder for brute-force fuzzy search
int bruteForceFuzzy(const char *pattern, const char *text);

// Levenshtein distance-based fuzzy search
int levenFuzzy(const char *s1, const char *s2);

#endif // FUZZY_H
