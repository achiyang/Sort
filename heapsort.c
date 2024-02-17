#include <stdlib.h>
#include <string.h>
#include "heapsort.h"

#define SWAP(a, b, temp, size) \
	do { \
		if (a == b) break; \
		memcpy(temp, a, size); \
		memcpy(a, b, size); \
		memcpy(b, temp, size); \
	} while(0)

static inline void heapify(char* arr, void* temp, size_t i, size_t n, size_t size_element, compareFunc compare) {
	size_t largest, left, right;
	char *a, *b, *p;

	largest = i;
	while (1) {
		left = largest * 2 + 1;
		right = left + 1;
		p = a = arr + (largest * size_element);

		if (left < n &&
			compare((b = arr + (left * size_element)), a) > 0) {
			largest = left;
			a = b;
		}

		if (right < n &&
			compare((b = arr + (right * size_element)), a) > 0) {
			largest = right;
			a = b;
		}

		if (a == p)
			break;

		SWAP(p, a, temp, size_element);
	}
}

void heapSort(void* arr, size_t num_elements, size_t size_element, compareFunc compare) {
	char *array;
	char *p;
	void *temp;

	if (num_elements > 1) {
		array = (char*)arr;
		temp = malloc(size_element);
		if (temp == NULL)
			exit(EXIT_FAILURE);

		for (size_t i = num_elements / 2; i > 0; --i) {
			heapify(array, temp, i - 1, num_elements, size_element, compare);
		}

		for (size_t i = num_elements - 1; i > 0; --i) {
			p = array + (i * size_element);
			SWAP(array, p, temp, size_element);
			heapify(array, temp, 0, i, size_element, compare);
		}

		free(temp);
	}
}
