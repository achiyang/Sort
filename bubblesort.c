#include "sortutils.h"
#include "bubblesort.h"

void bubbleSort(void *arr, size_t num_elements, size_t size_element, compareFunc compare) {
	char *array;
	char *temp;
	char *p;
	int swapped;

	if (num_elements > 1) {
		array = (char *)arr;
		temp = malloc(size_element);
		if (temp == NULL)
			exit(EXIT_FAILURE);

		for (int i = 0; i < num_elements - 1; i++) {
			swapped = 0;
			for (int j = 1; j < num_elements - i; j++) {
				p = array + j * size_element;
				if (compare(p, p - size_element) < 0) {
					SWAP(p, p - size_element, temp, size_element);
					swapped = 1;
				}
			}
			if (!swapped) break;
		}

		free(temp);
	}
}
