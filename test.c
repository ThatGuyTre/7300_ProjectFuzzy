#include "fuzzy.h"

int main() {
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

	return 0;
}
