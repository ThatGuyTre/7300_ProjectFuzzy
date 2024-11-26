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
			return 1; // Handle memory allocation failure
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

	if (patternLen != textLen)
	{
		return -1; // Length mismatch
	}

	int distance = 0;
	for (size_t i = 0; i < patternLen; i++)
	{
		if (pattern[i] != text[i])
		{
			distance++;
		}
	}
	return distance;
}

int levenFuzzy(const char *pattern, const char *text)
{
	int patternLen = strlen(pattern);
	int textLen = strlen(text);

	// Dynamically allocate the 2D array
	int **dp = (int **)malloc((patternLen + 1) * sizeof(int *));
	for (int i = 0; i <= patternLen; i++)
	{
		dp[i] = (int *)malloc((textLen + 1) * sizeof(int));
	}

	// Initialize the base cases
	for (int i = 0; i <= patternLen; i++)
	{
		dp[i][0] = i;
	}
	for (int j = 0; j <= textLen; j++)
	{
		dp[0][j] = j;
	}

	// Fill the dp array using the Levenshtein algorithm
	for (int i = 1; i <= patternLen; i++)
	{
		for (int j = 1; j <= textLen; j++)
		{
			int cost = (pattern[i - 1] == text[j - 1]) ? 0 : 1;
			dp[i][j] = fmin(dp[i - 1][j] + 1,				// Deletion
							fmin(dp[i][j - 1] + 1,			// Insertion
								 dp[i - 1][j - 1] + cost)); // Substitution
		}
	}

	// Store the result
	int result = dp[patternLen][textLen];

	// Free the allocated memory
	for (int i = 0; i <= patternLen; i++)
	{
		free(dp[i]);
	}
	free(dp);

	return result;
}
