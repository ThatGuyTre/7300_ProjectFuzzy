#include <time.h>
#include "fuzzy.h"
#include "sentence_search.h"

#define line_count 1600000

// Struct to store tweet indices and count
typedef struct
{
	int *indices;
	int count;
} SearchResult;

// Modify the findTweetsWithTerm function to return SearchResult
SearchResult findTweetsWithTerm(char **tweets, const char *searchTerm, int threshold, int (*fuzzyFunction)(const char *, const char *))
{
	SearchResult result;
	result.indices = (int *)malloc(line_count * sizeof(int));
	if (result.indices == NULL)
	{
		perror("Memory allocation failed for tweetIndices");
		result.count = 0;
		return result;
	}

	int count = 0;
	for (int i = 0; i < line_count; i++)
	{
		// If you have found more than 0 matches, store the index in the result
		if (sentenceSearch(tweets[i], searchTerm, threshold, fuzzyFunction) > 0)
		{
			result.indices[count] = i;
			count++;
		}
	}

	// Reallocate memory to fit the number of valid results
	result.indices = (int *)realloc(result.indices, count * sizeof(int));
	result.count = count;
	return result;
}

void loadTweets(char **tweets)
{
	// Load in the tweets.csv file
	FILE *fp = fopen("tweets.csv", "r");
	if (fp == NULL)
	{
		printf("Error: Could not open file\n");
		return;
	}

	// Read each line of the file and store it in the tweets array
	char line[1024];
	int index = 0;
	while (fgets(line, sizeof(line), fp) && index < line_count)
	{
		// Allocate memory for each tweet text
		tweets[index] = (char *)malloc(strlen(line) + 1);
		if (tweets[index] == NULL)
		{
			printf("Memory allocation failed for tweet %d\n", index);
			return;
		}
		strcpy(tweets[index], line); // Copy the line into the array
		index++;
	}
	fclose(fp);
}

// int main(int argc, char *argv[])
// {
// 	atexit(cleanupDpArray);

// 	// Load array for the tweet text
// 	char **tweets = (char **)malloc(line_count * sizeof(char *));
// 	loadTweets(tweets);

// 	// Search for tweets containing the term in command line argument
// 	char *term = argv[1];
// 	if (term == NULL)
// 	{
// 		printf("Please provide a search term\n");
// 		return 1;
// 	}

// 	clock_t start, end;
// 	double levenTime, hammingTime, bruteTime, bruteCyclicTime;
// 	const int threshold = 1;

// 	printf("Searching for tweets containing the term \"%s\"...\n", term);

// 	// Levenshtein search with threshold 1
// 	printf("Levenshtein fuzzy search...\n");
// 	start = clock();
// 	SearchResult levenResult = findTweetsWithTerm(tweets, term, threshold, levenFuzzy);
// 	end = clock();
// 	levenTime = ((double)(end - start)) / CLOCKS_PER_SEC;
// 	printf("Levenshtein search found: %d\n", levenResult.count);
// 	printf("Time taken for Levenshtein search: %f seconds\n", levenTime);

// 	// Hamming search with threshold 1
// 	printf("Hamming fuzzy search...\n");
// 	start = clock();
// 	SearchResult hammingResult = findTweetsWithTerm(tweets, term, threshold, hammingFuzzy);
// 	end = clock();
// 	hammingTime = ((double)(end - start)) / CLOCKS_PER_SEC;
// 	printf("Hamming search found: %d\n", hammingResult.count);
// 	printf("Time taken for Hamming search: %f seconds\n", hammingTime);

// 	// Bruteforce search (threshold doesn't matter)
// 	printf("Bruteforce fuzzy search...\n");
// 	start = clock();
// 	SearchResult bruteForceResult = findTweetsWithTerm(tweets, term, threshold, bruteForceFuzzy);
// 	end = clock();
// 	bruteTime = ((double)(end - start)) / CLOCKS_PER_SEC;
// 	printf("Brute-force search found: %d\n", bruteForceResult.count);
// 	printf("Time taken for Brute-force search: %f seconds\n", bruteTime);

// 	// Cyclic bruteforce search
// 	printf("Brute-force cyclic fuzzy search...\n");
// 	start = clock();
// 	SearchResult bruteForceCyclicResult = findTweetsWithTerm(tweets, term, threshold, bruteForceCyclicFuzzy);
// 	end = clock();
// 	bruteCyclicTime = ((double)(end - start)) / CLOCKS_PER_SEC;
// 	printf("Brute-force cyclic search found: %d\n", bruteForceCyclicResult.count);
// 	printf("Time taken for Brute-force cyclic search: %f seconds\n", bruteCyclicTime);

// 	// Free the allocated memory from tweets array
// 	for (int i = 0; i < line_count; i++)
// 	{
// 		free(tweets[i]);
// 	}
// 	free(tweets);

// 	// Free the allocated memory for search results
// 	free(levenResult.indices);
// 	free(hammingResult.indices);
// 	free(bruteForceResult.indices);
// 	free(bruteForceCyclicResult.indices);

// 	return 0;
// }

int main(int argc, char *argv[])
{
	atexit(cleanupDpArray);

	// Load array for the tweet text
	char **tweets = (char **)malloc(line_count * sizeof(char *));
	loadTweets(tweets);

	if (argc < 2)
	{
		printf("Please provide at least one search term\n");
		return 1;
	}

	// Iterate over each provided search term
	for (int termIdx = 1; termIdx < argc; termIdx++)
	{
		char *term = argv[termIdx];

		clock_t start, end;
		double bruteCyclicTime;

		start = clock();
		SearchResult bruteForceCyclicResult = findTweetsWithTerm(tweets, term, 1, bruteForceCyclicFuzzy);
		end = clock();
		bruteCyclicTime = ((double)(end - start)) / CLOCKS_PER_SEC;

		// Print results in the specified format
		printf("{%s, %d, %.6f}\n", term, bruteForceCyclicResult.count, bruteCyclicTime);

		free(bruteForceCyclicResult.indices);
	}

	// Free the allocated memory from tweets array
	for (int i = 0; i < line_count; i++)
	{
		free(tweets[i]);
	}
	free(tweets);

	return 0;
}
