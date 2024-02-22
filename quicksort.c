#include "sortutils.h"
#include "quicksort.h"

#define THRESHOLD 64

static size_t partition(char *arr, char *temp, size_t left, size_t right, size_t size_element, compareFunc compare) {
	char *l, *r, *p;

	l = arr + left * size_element;
	p = arr + ((left + right) >> 1) * size_element;
	r = arr + right * size_element;
	if (compare(p, l) < 0) {
		SWAP(p, l, temp, size_element);
	}
	if (compare(r, p) < 0) {
		SWAP(r, p, temp, size_element);
		if (compare(p, l) < 0) {
			SWAP(p, l, temp, size_element);
		}
	}
	r -= size_element;
	SWAP(p, r, temp, size_element);
	p = r;

	while (1) {
		l += size_element;
		r -= size_element;
		while (compare(l, p) <= 0 && l <= r) {
			l += size_element;
		}
		while (compare(r, p) > 0 && l <= r) {
			r -= size_element;
		}
		if (l < r) {
			SWAP(l, r, temp, size_element);
		}
		else {
			break;
		}
	}
	SWAP(p, l, temp, size_element);

	return (l - arr) / size_element;
}

static void quickSortCore(char *arr, char *temp, size_t left, size_t right, size_t size_element, compareFunc compare) {
	if (right - left > THRESHOLD) {
		size_t mid = partition(arr, temp, left, right, size_element, compare);
		size_t i = mid;
		for (; compare(arr + i * size_element, arr + (i - 1) * size_element) == 0 && i > left + 1; --i);
		quickSortCore(arr, temp, left, i - 1, size_element, compare);
		quickSortCore(arr, temp, mid + 1, right, size_element, compare);
	}
	else {
		insertSort(arr, temp, left, right, size_element, compare);
	}
}

void quickSort(void *arr, size_t num_elements, size_t size_element, compareFunc compare) {
	char *temp;

	if (num_elements > 1) {
		temp = malloc(THRESHOLD * size_element);
		if (temp == NULL)
			exit(EXIT_FAILURE);
		quickSortCore((char *)arr, temp, 0, num_elements - 1, size_element, compare);

		free(temp);
	}
}
