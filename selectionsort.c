#include "sortutils.h"
#include "selectionsort.h"

void selectionSort(void *arr, size_t num_elements, size_t size_element, compareFunc compare) {
	char *array;
	char *temp;
	char *p, *min, *curr;

	if (num_elements > 1) {
		array = (char *)arr;
		temp = malloc(size_element);
		if (temp == NULL)
			exit(EXIT_FAILURE);

		for (int i = 0; i < num_elements - 1; i++) {
			p = min = array + i * size_element;

			for (int j = i; j < num_elements; j++) {
				curr = array + j * size_element;
				if (compare(min, curr) > 0) {
					min = curr;
				}
			}

			if (p != min) {
				SWAP(p, min, temp, size_element);
			}
		}

		free(temp);
	}
}
