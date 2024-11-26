#include "fuzzy.h"

int main() {
	const char *str1 = "kitten";
	const char *str2 = "sitting";
	const char *str3 = "fluffy";
	const char *str4 = "fuzzy";

	printf("Levenshtein distance between \"%s\" and \"%s\": %d\n", str1, str2, levenFuzzy(str1, str2));
	printf("Levenshtein distance between \"%s\" and \"%s\": %d\n", str3, str4, levenFuzzy(str3, str4));
	printf("Levenshtein distance between \"%s\" and \"%s\": %d\n", str1, str4, levenFuzzy(str1, str4));

	return 0;
}
