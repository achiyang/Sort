#include <stdio.h>
#include <stdlib.h>
#include "rbtree.h"

void freeRBTree(RBNode *root) {
	if (root != NULL) {
		freeRBTree(root->left);
		freeRBTree(root->right);
		free(root);
	}
}

static void leftRotation(RBNode **rootRef, RBNode *node) {
	RBNode *child = node->right;

	node->right = child->left;
	if (child->left != NULL) {
		child->left->parent = node;
	}

	child->parent = node->parent;
	if (node->parent == NULL) {
		*rootRef = child;
	}
	else if (node == node->parent->left) {
		node->parent->left = child;
	}
	else if (node == node->parent->right) {
		node->parent->right = child;
	}
	else {
		fprintf(stderr, "node is not a child of node->parent\n");
		exit(EXIT_FAILURE);
	}

	child->left = node;
	node->parent = child;
}

static void rightRotation(RBNode **rootRef, RBNode *node) {
	RBNode *child = node->left;

	node->left = child->right;
	if (child->right != NULL) {
		child->right->parent = node;
	}

	child->parent = node->parent;
	if (node->parent == NULL) {
		*rootRef = child;
	}
	else if (node == node->parent->left) {
		node->parent->left = child;
	}
	else if (node == node->parent->right) {
		node->parent->right = child;
	}
	else {
		fprintf(stderr, "node is not a child of node->parent\n");
		exit(EXIT_FAILURE);
	}

	child->right = node;
	node->parent = child;
}

static void fixInsert(RBNode **rootRef, RBNode *node) {
	while (node->parent != NULL && node->parent->color == 'r') {
		if (node->parent == node->parent->parent->left) {
			RBNode *uncle = node->parent->parent->right;
			if (uncle != NULL && uncle->color == 'r') {
				node->parent->color = 'b';
				uncle->color = 'b';
				node->parent->parent->color = 'r';
				node = node->parent->parent;
			}
			else {
				if (node == node->parent->right) {
					node = node->parent;
					leftRotation(rootRef, node);
				}
				node->parent->color = 'b';
				node->parent->parent->color = 'r';
				rightRotation(rootRef, node->parent->parent);
			}
		}
		else {
			RBNode *uncle = node->parent->parent->left;
			if (uncle != NULL && uncle->color == 'r') {
				node->parent->color = 'b';
				uncle->color = 'b';
				node->parent->parent->color = 'r';
				node = node->parent->parent;
			}
			else {
				if (node == node->parent->left) {
					node = node->parent;
					rightRotation(rootRef, node);
				}
				node->parent->color = 'b';
				node->parent->parent->color = 'r';
				leftRotation(rootRef, node->parent->parent);
			}
		}
	}
	(*rootRef)->color = 'b';
}

static RBNode *createRBNode(void *data) {
	RBNode *node = (RBNode *)malloc(sizeof(RBNode));
	if (node == NULL)
		exit(EXIT_FAILURE);

	node->data = data;
	node->color = 'r';
	node->left = node->right = node->parent = NULL;
	return node;
}

void insertRBTree(RBNode **rootRef, void *data, compareFunc compare) {
	RBNode *newNode = createRBNode(data);

	if (*rootRef == NULL) {
		newNode->color = 'b';
		*rootRef = newNode;
		return;
	}

	RBNode *parent = NULL;
	RBNode *current = *rootRef;

	while (current != NULL) {
		parent = current;
		if (compare(data, current->data) < 0) {
			current = current->left;
		}
		else {
			current = current->right;
		}
	}

	if (compare(data, parent->data) < 0) {
		parent->left = newNode;
	}
	else {
		parent->right = newNode;
	}
	newNode->parent = parent;

	fixInsert(rootRef, newNode);
}
