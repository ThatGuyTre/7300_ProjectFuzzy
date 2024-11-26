#include <math.h>
#include <stdio.h>
#include <string.h>
#include "fuzzy.h"

// HammingFuzzy and LevenFuzzy functions adapted from https://github.com/tkarabela/bigpython/blob/master/003--fuzzy-text-search/fuzzy-text-search.ipynb

int hammingFuzzy(const char *pattern, const char *text) {
	size_t pattern_len = strlen(pattern);
	size_t text_len = strlen(text);

	if (pattern_len != text_len) {
		return -1; // Lengths are not equal, return error
	}

	int distance = 0;

	// Calculate the Hamming distance
	for (size_t i = 0; i < pattern_len; i++) {
		if (pattern[i] != text[i]) {
			distance++;
		}
	}

	return distance;
}

int bruteForceFuzzy(const char *pattern, const char *text) {
	int pattern_len = strlen(pattern);
	int text_len = strlen(text);

	if(pattern_len > text_len) {
		// Pattern must be shorter than text
		return -1;
	}

	// Create an array of all possible 2-letter swaps
    char swapped_patterns[pattern_len * pattern_len][pattern_len + 1]; // Store swapped strings
    int swap_count = 0;

    for (int i = 0; i < pattern_len; i++) {
        for (int j = i + 1; j < pattern_len; j++) {
            strcpy(swapped_patterns[swap_count], pattern);  // Copy the whole pattern
            // Swap the characters at indices i and j
            char temp = swapped_patterns[swap_count][i];
            swapped_patterns[swap_count][i] = swapped_patterns[swap_count][j];
            swapped_patterns[swap_count][j] = temp;
            swap_count++;
        }
    }

	// Compare each shifted pattern with the text
	for(int i = 0; i < text_len - pattern_len + 1; i++) {
		for(int j = 0; j < pattern_len; j++) {
			if(text[i + j] != swapped_patterns[j][j]) {
				break;
			}
			if(j == pattern_len - 1) {
				return i;
			}
		}
	}

	// If you still can't find a match, return -1
	// but in the future we may want to search substrings of the swapped_patterns array
	
	return -1;
}

int levenFuzzy(const char *s1, const char *s2) {
	int len1 = strlen(s1);
	int len2 = strlen(s2);
	int dp[len1 + 1][len2 + 1];

	for (int i = 0; i <= len1; i++)
		dp[i][0] = i;
	for (int j = 0; j <= len2; j++)
		dp[0][j] = j;

	for (int i = 1; i <= len1; i++) {
		for (int j = 1; j <= len2; j++) {
			int cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;
			dp[i][j] = fmin(dp[i - 1][j] + 1,       // Deletion
						fmin(dp[i][j - 1] + 1,      // Insertion
							dp[i - 1][j - 1] + cost)); // Substitution
		}
	}
	return dp[len1][len2];
}
