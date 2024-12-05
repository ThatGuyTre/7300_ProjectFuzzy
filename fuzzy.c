#include "fuzzy.h"
#include "suffixtree.h"
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

int bruteForceCyclicFuzzy(const char* query, const char* tweet){
	/*
		You can indeed create an index by taking all cyclic suffixes of each word and create a suffix tree. Now, for the query word, take all its cyclic permutations and search each of these m strings in the circular suffix tree. Thus, you will no longer be going word by word.
 
		For example, if query word is abcaba$, then for each position omitted our cyclic suffixes are
		bcaba$
		caba$a
		aba$ab
		ba$abc
		a$abca
		$abcab
		
		$ is just for you to see what’s happening, in the real index omit $.
		
		What we achieved here is that we circularly pushed the error character to the end of the string and then omitted it.
	*/

	size_t patternLen = strlen(query);
	size_t textLen = strlen(tweet);

	// Create cyclic suffixes of the tweet
	char **cyclicSuffixes = (char **)malloc(textLen * sizeof(char *));
	for (size_t i = 0; i < textLen; i++) {
		cyclicSuffixes[i] = (char *)malloc((textLen + 1) * sizeof(char));
		for (size_t j = 0; j < textLen; j++) {
			cyclicSuffixes[i][j] = tweet[(i + j) % textLen];
		}
		cyclicSuffixes[i][textLen] = '\0';
	}

	// Build the suffix tree for the cyclic suffixes
	SuffixTreeNode *suffixTreeRoot = buildSuffixTree(tweet);

	// Print the traversal of the suffix tree
	printf("Traversal of the suffix tree:\n");
	printSuffixTree(suffixTreeRoot, tweet);

	if (patternLen > textLen) {
		return -1; // Pattern longer than text
	}

	// Create cyclic permutations of the pattern
	char **cyclicPermutations = (char **)malloc(patternLen * sizeof(char *));
	for (size_t i = 0; i < patternLen; i++) {
		cyclicPermutations[i] = (char *)malloc((patternLen + 1) * sizeof(char));
		for (size_t j = 0; j < patternLen; j++) {
			cyclicPermutations[i][j] = query[(i + j) % patternLen];
		}
		cyclicPermutations[i][patternLen] = '\0';
	}

	// Search for each cyclic permutation in the text
	for (size_t k = 0; k < patternLen; k++) {
		for (size_t i = 0; i <= textLen - patternLen; i++) {
			size_t j = 0;
			while (j < patternLen && cyclicPermutations[k][j] == tweet[i + j]) {
				j++;
			}
			if (j == patternLen) {
				for (size_t l = 0; l < patternLen; l++) {
					free(cyclicPermutations[l]);
				}
				free(cyclicPermutations);
				return i; // Match found at position i
			}
		}
	}

	// Free allocated memory
	for (size_t i = 0; i < patternLen; i++) {
		free(cyclicPermutations[i]);
	}
	free(cyclicPermutations);

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

	// Ensure the dp array is large enough
	resizeDpArray(patternLen + 1, textLen + 1);

	// Initialize the base cases
	for (int i = 0; i <= patternLen; i++) {
		dpArray.dp[i][0] = i;
	}
	for (int j = 0; j <= textLen; j++) {
		dpArray.dp[0][j] = j;
	}

	// Fill the dp array using the Levenshtein algorithm
	for (int i = 1; i <= patternLen; i++) {
		for (int j = 1; j <= textLen; j++) {
			int cost = (pattern[i - 1] == text[j - 1]) ? 0 : 1;
			dpArray.dp[i][j] = fmin(dpArray.dp[i - 1][j] + 1,                  // Deletion
									fmin(dpArray.dp[i][j - 1] + 1,             // Insertion
										dpArray.dp[i - 1][j - 1] + cost));    // Substitution
		}
	}

	// Store the result
	int result = dpArray.dp[patternLen][textLen];

	return result;
}
