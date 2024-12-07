#include "fuzzy.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

int bruteForceFuzzy(const char *pattern, const char *text)
{
	size_t patternLen = strlen(pattern);
	size_t textLen = strlen(text);

	if(patternLen > textLen)
	{
		return -1; // Pattern longer than text
	}

	for (size_t i = 0; i <= textLen - patternLen; i++)
	{
		size_t j = 0;

		// Check for direct match
		while (j < patternLen && pattern[j] == text[i + j])
		{
			j++;
		}

		if (j == patternLen)
		{
			return i; // Match found at position i
		}

		// Use a mutable copy of the pattern
		char *clonedPattern = (char *)malloc(strlen(pattern) + 1); // Allocate memory
		if (clonedPattern == NULL) {
			printf("Memory allocation failed");
			return -1; // Handle memory allocation failure
		}
		strcpy(clonedPattern, pattern);

		// Try swapping adjacent characters in the cloned pattern
		for (size_t swapIdx = 0; swapIdx < patternLen - 1; swapIdx++)
		{
			if (clonedPattern[swapIdx] != text[i + swapIdx])
			{
				// Swap character with the next one
				char temp = clonedPattern[swapIdx];
				clonedPattern[swapIdx] = clonedPattern[swapIdx + 1];
				clonedPattern[swapIdx + 1] = temp;

				j = 0;
				while (j < patternLen && clonedPattern[j] == text[i + j])
				{
					j++;
				}

				// Restore the original order in the cloned pattern
				clonedPattern[swapIdx + 1] = clonedPattern[swapIdx];
				clonedPattern[swapIdx] = temp;

				if (j == patternLen)
				{
					free(clonedPattern); // Free the duplicated string
					return i;			 // Match found after swap
				}
			}
		}

		free(clonedPattern); // Free the duplicated string after use
	}
	return -1; // No match found
}

int hammingFuzzy(const char *pattern, const char *text)
{
	size_t patternLen = strlen(pattern);
	size_t textLen = strlen(text);

	if (patternLen > textLen)
	{
		return -1; // pattern longer than text
	}

	int minDistance = patternLen + 1;
	//slides pattern 'window' over sentence.
	for (size_t i = 0; i < textLen - patternLen; i++)
	{
		for (size_t i = 0; i <= textLen - patternLen; i++) {
			int distance = 0;
			for (size_t j = 0; j < patternLen; j++) {
				if(pattern[j] != text[i + j]) {
					distance++;
				}
			}
			if (distance < minDistance) {
				minDistance = distance;
			}
		}
	}
	return minDistance <= patternLen ? minDistance : -1;
}

typedef struct {
	int **dp;
	int rows;
	int cols;
} DPArray;

// Singleton instance for dp array
static DPArray dpArray = {NULL, 0, 0};

// Function to resize the dp array if needed
void resizeDpArray(int rows, int cols) {
	if (rows > dpArray.rows || cols > dpArray.cols) {
		// Free existing dp array
		if (dpArray.dp) {
			for (int i = 0; i < dpArray.rows; i++) {
				free(dpArray.dp[i]);
			}
			free(dpArray.dp);
		}

		// Allocate new dp array
		dpArray.dp = (int **)malloc(rows * sizeof(int *));
		for (int i = 0; i < rows; i++) {
			dpArray.dp[i] = (int *)malloc(cols * sizeof(int));
		}
		dpArray.rows = rows;
		dpArray.cols = cols;
	}
}

// Cleanup function to free the dp array
void cleanupDpArray() {
	if (dpArray.dp) {
		for (int i = 0; i < dpArray.rows; i++) {
			free(dpArray.dp[i]);
		}
		free(dpArray.dp);
		dpArray.dp = NULL;
		dpArray.rows = 0;
		dpArray.cols = 0;
	}
}

// Levenshtein distance function
int levenFuzzy(const char *pattern, const char *text) {
	int patternLen = strlen(pattern);
	int textLen = strlen(text);

	if (patternLen > textLen) {
        return -1; // Pattern longer than text
    }

	int minDistance = patternLen + 1;

	// Ensure the dp array is large enough
	for (int start = 0; start <= textLen - patternLen; start++) {
		resizeDpArray(patternLen + 1, textLen + 1);
		//printf("Comparing pattern: '%s' with substring: '%.*s'\n", pattern, patternLen, &text[start]);

		// Initialize the base cases
		for (int i = 0; i <= patternLen; i++) {
			dpArray.dp[i][0] = i;
		}
		for (int j = 0; j <= patternLen; j++) {
			dpArray.dp[0][j] = j;
		}

		// Fill the dp array using the Levenshtein algorithm
		for (int i = 1; i <= patternLen; i++) {
			for (int j = 1; j <= patternLen; j++) {
				int cost = (pattern[i - 1] == text[start + j - 1]) ? 0 : 1;
				dpArray.dp[i][j] = fmin(dpArray.dp[i - 1][j] + 1,               // Deletion
									fmin(dpArray.dp[i][j - 1] + 1,             // Insertion
										dpArray.dp[i - 1][j - 1] + cost));    // Substitution
			}
		}

		int distance = dpArray.dp[patternLen][patternLen];
		if (distance < minDistance) {
			minDistance = distance;
		}
	}

	return minDistance;
}
