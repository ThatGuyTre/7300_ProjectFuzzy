#include "fuzzy.h"
#include "sentence_search.h"

#define line_count 1600000

// Struct to store tweet indices and count
typedef struct {
	int *indices;
	int count;
} SearchResult;

// Modify the findTweetsWithTerm function to return SearchResult
SearchResult findTweetsWithTerm(char **tweets, const char *searchTerm, int (*fuzzyFunction)(const char *, const char *)) {
	SearchResult result;
	result.indices = (int *)malloc(line_count * sizeof(int));
	if (result.indices == NULL) {
		perror("Memory allocation failed for tweetIndices");
		result.count = 0;
		return result;
	}

	int count = 0;
	for (int i = 0; i < line_count; i++) {
		// If you have found more than 0 matches, store the index in the result
		if (sentenceSearch(tweets[i], searchTerm, fuzzyFunction) > 0) {
			result.indices[count] = i;
			count++;
		}
	}

	// Reallocate memory to fit the number of valid results
	result.indices = (int *)realloc(result.indices, count * sizeof(int));
	result.count = count;
	return result;
}

void loadTweets(char **tweets) {
	// Load in the tweets.csv file
	FILE *fp = fopen("tweets.csv", "r");
	if (fp == NULL) {
		printf("Error: Could not open file\n");
		return;
	}

	// Read each line of the file and store it in the tweets array
	char line[1024];
	int index = 0;
	while (fgets(line, sizeof(line), fp) && index < line_count) {
		// Allocate memory for each tweet text
		tweets[index] = (char *)malloc(strlen(line) + 1);
		if (tweets[index] == NULL) {
			printf("Memory allocation failed for tweet %d\n", index);
			return;
		}
		strcpy(tweets[index], line); // Copy the line into the array
		index++;
	}
	fclose(fp);
}

int main(int argc, char *argv[]) {
	// Load array for the tweet text
	char **tweets = (char **)malloc(line_count * sizeof(char *));
	loadTweets(tweets);

	// Search for tweets containing the term "hello"
	char* term = argv[1];
	if(term == NULL){
		printf("Please provide a search term\n");
		return 1;
	}

	printf("Searching for tweets containing the term \"%s\"...\n", term);
	printf("Levenshtein fuzzy search...\n");
	SearchResult levenResult = findTweetsWithTerm(tweets, term, levenFuzzy);
	printf("Hamming fuzzy search...\n");
	SearchResult hammingResult = findTweetsWithTerm(tweets, term, hammingFuzzy);
	printf("Bruteforce fuzzy search...\n");
	SearchResult bruteForceResult = findTweetsWithTerm(tweets, term, bruteForceFuzzy);

	// Print the number of tweets found for each search
	printf("Levenshtein search found %d tweets\n", levenResult.count);
	printf("Hamming search found %d tweets\n", hammingResult.count);
	printf("Brute-force search found %d tweets\n", bruteForceResult.count);

	printf("Tweets containing the term \"%s\":\n", term);
	// Output all array contents for each search result
	// (Uncomment to print the tweet indices or tweet contents)
	
	for (int i = 0; i < levenResult.count; i++) {
		printf("Levenshtein match at index %d: %s\n", levenResult.indices[i], tweets[levenResult.indices[i]]);
	}
	for (int i = 0; i < hammingResult.count; i++) {
		printf("Hamming match at index %d: %s\n", hammingResult.indices[i], tweets[hammingResult.indices[i]]);
	}
	for (int i = 0; i < bruteForceResult.count; i++) {
		printf("Brute-force match at index %d: %s\n", bruteForceResult.indices[i], tweets[bruteForceResult.indices[i]]);
	}

	// Free the allocated memory from tweets array
	for (int i = 0; i < line_count; i++) {
		free(tweets[i]);
	}
	free(tweets);

	// Free the allocated memory for search results
	free(levenResult.indices);
	free(hammingResult.indices);
	free(bruteForceResult.indices);

	return 0;
}

void randomTests(){
	const char *str1 = "kitten";
	const char *str2 = "sitting";
	const char *str3 = "fluffy";
	const char *str4 = "fuzzy";
	const char *str5 = "fluzzy";
	const char *str6 = "kittens";

	printf("Levenshtein distance between \"%s\" and \"%s\": %d\n", str1, str2, levenFuzzy(str1, str2));
	printf("Levenshtein distance between \"%s\" and \"%s\": %d\n", str3, str4, levenFuzzy(str3, str4));
	printf("Levenshtein distance between \"%s\" and \"%s\": %d\n", str1, str4, levenFuzzy(str1, str4));
	printf("Levenshtein distance between \"%s\" and \"%s\": %d\n", str1, str1, levenFuzzy(str1, str1));
	printf("Levenshtein distance between \"%s\" and \"%s\": %d\n", str1, str6, levenFuzzy(str1, str6));

	printf("Hamming distance between \"%s\" and \"%s\": %d\n", str3, str4, hammingFuzzy(str3, str4));
	printf("Hamming distance between \"%s\" and \"%s\": %d\n", str1, str5, hammingFuzzy(str1, str5));
	printf("Hamming distance between \"%s\" and \"%s\": %d\n", str1, str2, hammingFuzzy(str1, str2));
	printf("Hamming distance between \"%s\" and \"%s\": %d\n", str1, str1, hammingFuzzy(str1, str1));
	printf("Hamming distance between \"%s\" and \"%s\": %d\n", str1, str6, hammingFuzzy(str1, str6));

	printf("Bruteforce match between \"%s\" and \"%s\": %d\n", str4, str3, bruteForceFuzzy(str4, str3));
	printf("Bruteforce match between \"%s\" and \"%s\": %d\n", str4, str1, bruteForceFuzzy(str4, str1));
	printf("Bruteforce match between \"%s\" and \"%s\": %d\n", str1, str2, bruteForceFuzzy(str1, str2));
	printf("Bruteforce match between \"%s\" and \"%s\": %d\n", str1, str1, bruteForceFuzzy(str1, str1));
	printf("Bruteforce match between \"%s\" and \"%s\": %d\n", str1, str6, bruteForceFuzzy(str1, str6));
}
