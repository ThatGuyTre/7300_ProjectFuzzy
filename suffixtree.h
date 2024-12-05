#ifndef SUFFIXTREE_H
#define SUFFIXTREE_H

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
SuffixTreeNode *newNode(int start, int *end);

// Function to build the suffix tree for the given text
SuffixTreeNode *buildSuffixTree(const char *text);

// Function to print the suffix tree
void printSuffixTree(SuffixTreeNode *node, const char *text);

#endif // SUFFIXTREE_H
