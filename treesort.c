#include "sortutils.h"
#include "rbtree.h"
#include "treesort.h"

static void order(RBNode *root, void *arr, size_t size, int *i) {
	if (root != NULL) {
		order(root->left, arr, size, i);
		memcpy((char *)arr + (*i)++ * size, root->data, size);
		order(root->right, arr, size, i);
	}
}

void treeSort(void *arr, size_t num_elements, size_t size_element, compareFunc compare) {
	char *temp;
	RBNode *root;

	if (num_elements > 1) {
		temp = malloc(num_elements * size_element);
		if (temp == NULL)
			exit(EXIT_FAILURE);
		memcpy(temp, arr, num_elements * size_element);

		root = NULL;
		for (size_t i = 0; i < num_elements; i++) {
			insertRBTree(&root, temp + i * size_element, compare);
		}

		int i = 0;
		order(root, arr, size_element, &i);

		freeRBTree(root);
		free(temp);
	}
}
