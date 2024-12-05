#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct SuffixTreeNode {
	struct SuffixTreeNode *children[256]; // Assuming ASCII characters
	int start;
	int *end;
	struct SuffixTreeNode *suffixLink;
} SuffixTreeNode;

// Function to create a new tree node
SuffixTreeNode *newNode(int start, int *end) {
	SuffixTreeNode *node = (SuffixTreeNode *)malloc(sizeof(SuffixTreeNode));
	for (int i = 0; i < 256; i++) {
		node->children[i] = NULL;
	}
	node->start = start;
	node->end = end;
	node->suffixLink = NULL;
	return node;
}

// Function to build the suffix tree for the given text
SuffixTreeNode *buildSuffixTree(const char *text) {
	int n = strlen(text);
	int *rootEnd = (int *)malloc(sizeof(int));
	*rootEnd = -1;
	SuffixTreeNode *root = newNode(-1, rootEnd);

	SuffixTreeNode *activeNode = root;
	int activeEdge = -1;
	int activeLength = 0;
	int remainingSuffixCount = 0;
	int *leafEnd = (int *)malloc(sizeof(int));
	*leafEnd = -1;
	int *splitEnd = NULL;

	for (int i = 0; i < n; i++) {
		*leafEnd = i;
		remainingSuffixCount++;
		while (remainingSuffixCount > 0) {
			if (activeLength == 0) {
				activeEdge = i;
			}
			if (activeNode->children[text[activeEdge]] == NULL) {
				activeNode->children[text[activeEdge]] = newNode(i, &leafEnd);
				if (activeNode != root) {
					activeNode = activeNode->suffixLink;
				}
			} else {
				SuffixTreeNode *next = activeNode->children[text[activeEdge]];
				int edgeLength = *(next->end) - next->start + 1;
				if (activeLength >= edgeLength) {
					activeEdge += edgeLength;
					activeLength -= edgeLength;
					activeNode = next;
					continue;
				}
				if (text[next->start + activeLength] == text[i]) {
					activeLength++;
					break;
				}
				splitEnd = (int *)malloc(sizeof(int));
				*splitEnd = next->start + activeLength - 1;
				SuffixTreeNode *split = newNode(next->start, splitEnd);
				activeNode->children[text[activeEdge]] = split;
				split->children[text[i]] = newNode(i, &leafEnd);
				next->start += activeLength;
				split->children[text[next->start]] = next;
				if (activeNode != root) {
					activeNode = activeNode->suffixLink;
				}
			}
			remainingSuffixCount--;
		}
	}
	free(leafEnd);
	return root;
}

void printSuffixTree(SuffixTreeNode *node, const char *text) {
	if (node == NULL) {
		return;
	}

	if (node->start != -1) {
		int length = *(node->end) - node->start + 1;
		char *buffer = (char *)malloc(length + 1);
		strncpy(buffer, text + node->start, length);
		buffer[length] = '\0';
		printf("%s\n", buffer);
		free(buffer);
	}

	for (int i = 0; i < 256; i++) {
		if (node->children[i] != NULL) {
			printSuffixTree(node->children[i], text);
		}
	}
}