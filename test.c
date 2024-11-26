#include "fuzzy.h"

#define line_count 1600000

void loadTweets(char **tweets){

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

int main() {

	// Load array for the tweet text
	char **tweets = (char **)malloc(line_count * sizeof(char *));
	loadTweets(tweets);

	// Print out the first 10 elements of the tweets array
    for (int i = 0; i < 10; i++) {
        printf("%s", tweets[i]);
    }

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

	// Free the allocated memory from tweets array
    for (int i = 0; i < line_count; i++) {
        free(tweets[i]);
    }
    free(tweets);

	return 0;
}
