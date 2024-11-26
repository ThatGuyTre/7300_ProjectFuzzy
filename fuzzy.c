#include "fuzzy.h"
#include "math.h"

int hammingFuzzy(const char *pattern, const char *text) {
    // Placeholder for Hamming distance implementation
    printf("Hamming fuzzy search is not yet implemented.\n");
	return -1;
}

int bruteForceFuzzy(const char *pattern, const char *text) {
    // Placeholder for brute-force fuzzy search implementation
    printf("Brute force fuzzy search is not yet implemented.\n");
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
