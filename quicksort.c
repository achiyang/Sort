#include <stdlib.h>
#include <string.h>
#include "quicksort.h"

#define THRESHOLD 64
#define SWAP(a, b, temp, size) \
	do { \
		memcpy(temp, a, size); \
		memcpy(a, b, size); \
		memcpy(b, temp, size); \
	} while(0)

static void insertSort(char *arr, char *temp, size_t left, size_t right, size_t size_element, compareFunc compare) {
	size_t i;
	size_t temp_left, temp_right, temp_mid;
	char *a, *b;

	for (i = left + 1; i <= right; ++i) {
		a = arr + i * size_element;
		if (compare(a, a - size_element) < 0) {
			temp_left = left;
			temp_right = i - 1;
			while (temp_left < temp_right) {
				temp_mid = (temp_left + temp_right) >> 1;
				if (compare(a, arr + temp_mid * size_element) < 0) {
					temp_right = temp_mid;
				}
				else {
					temp_left = temp_mid + 1;
				}
			}
			b = arr + temp_left * size_element;
			memcpy(temp, b, a - b);
			memcpy(b, a, size_element);
			memcpy(b + size_element, temp, a - b);
		}
	}
}

static size_t partition(char *arr, char *temp, size_t left, size_t right, size_t size_element, compareFunc compare) {
	char *lp, *rp, *p;

	lp = arr + left * size_element;
	p = arr + ((left + right) >> 1) * size_element;
	rp = arr + right * size_element;
	if (compare(p, lp) < 0) {
		SWAP(p, lp, temp, size_element);
	}
	if (compare(rp, p) < 0) {
		SWAP(rp, p, temp, size_element);
		if (compare(p, lp) < 0) {
			SWAP(p, lp, temp, size_element);
		}
	}
	rp -= size_element;
	SWAP(p, rp, temp, size_element);
	p = rp;

	while (1) {
		lp += size_element;
		rp -= size_element;
		while (lp <= rp && compare(lp, p) <= 0) {
			lp += size_element;
		}
		while (lp <= rp && compare(rp, p) > 0) {
			rp -= size_element;
		}
		if (lp < rp) {
			SWAP(lp, rp, temp, size_element);
		}
		else {
			break;
		}
	}
	if (lp != p) {
		SWAP(p, lp, temp, size_element);
	}

	return (lp - arr) / size_element;
}

static void quickSortCore(char *arr, char *temp, size_t left, size_t right, size_t size_element, compareFunc compare) {
	if (right - left > THRESHOLD) {
		size_t mid = partition(arr, temp, left, right, size_element, compare);
		quickSortCore(arr, temp, left, mid - 1, size_element, compare);
		quickSortCore(arr, temp, mid + 1, right, size_element, compare);
	}
	else {
		insertSort(arr, temp, left, right, size_element, compare);
	}
}

void quickSort(void *arr, size_t num_elements, size_t size_element, compareFunc compare) {
	char *temp;

	temp = malloc(THRESHOLD * size_element);
	if (temp == NULL)
		exit(EXIT_FAILURE);
	quickSortCore((char *)arr, temp, 0, num_elements - 1, size_element, compare);

	free(temp);
}
