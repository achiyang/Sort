#ifndef RBTREE_H
#define RBTREE_H

#include "sorttypes.h"

typedef struct RBNode {
	void *data;
	char color; // 'r' for red, 'b' for black
	struct RBNode* parent;
	struct RBNode* left;
	struct RBNode* right;
} RBNode;

void freeRBTree(RBNode* root);

void insertRBTree(RBNode **rootRef, void *data, compareFunc compare);

#endif // RBTREE_H
