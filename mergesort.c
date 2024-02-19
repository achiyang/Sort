#include "sortutils.h"
#include "mergesort.h"

#define THRESHOLD 64

static void mergeSortCore(char *arr, char *temp, size_t left, size_t right, size_t size_element, compareFunc compare) {
	size_t mid;

	if (left < right) {
		if (right - left > THRESHOLD) {
			mid = (left + right) >> 1;
			mergeSortCore(arr, temp, left, mid, size_element, compare);
			mergeSortCore(arr, temp, mid + 1, right, size_element, compare);
			merge(arr, temp, left, mid, right, size_element, compare);
		}
		else {
			insertSort(arr, temp, left, right, size_element, compare);
		}
	}
}

void mergeSort(void *arr, size_t num_elements, size_t size_element, compareFunc compare) {
	char *temp;

	if (num_elements > 1) {
		temp = (char *)malloc(size_element * MAX(num_elements >> 1, THRESHOLD));
		if (temp == NULL)
			exit(EXIT_FAILURE);

		mergeSortCore((char *)arr, temp, 0, num_elements - 1, size_element, compare);

		free(temp);
	}
}
