#include "sortutils.h"
#include "insertionsort.h"

void insertionSort(void *arr, size_t num_elements, size_t size_element, compareFunc compare) {
	char *temp;

	if (num_elements > 1) {
		temp = malloc((num_elements - 1) * size_element);
		if (temp == NULL)
			exit(EXIT_FAILURE);
		insertSort((char *)arr, temp, 0, num_elements - 1, size_element, compare);
		free(temp);
	}
}
