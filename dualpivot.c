#include "sortutils.h"
#include "dualpivot.h"

#define THRESHOLD 64

typedef struct Pair {
	size_t first;
	size_t second;
} Pair;

static Pair partition(char *arr, char *temp, size_t left, size_t right, size_t size_element, compareFunc compare) {
	size_t size_one_third;
	char *lp, *rp;
	char *l, *r, *k;

	lp = arr + left * size_element;
	rp = arr + right * size_element;

	size_one_third = (right - left) / 3 * size_element;
	l = lp + size_one_third;
	r = l + size_one_third;
	if (compare(lp, l) > 0) {
		SWAP(lp, l, temp, size_element);
	}
	if (compare(l, r) > 0) {
		SWAP(l, r, temp, size_element);
		if (compare(lp, l) > 0) {
			SWAP(lp, l, temp, size_element);
		}
	}
	if (compare(r, rp) > 0) {
		SWAP(r, rp, temp, size_element);
		if (compare(l, r) > 0) {
			SWAP(l, r, temp, size_element);
			if (compare(lp, l) > 0) {
				SWAP(lp, l, temp, size_element);
			}
		}
	}
	lp += size_element;
	rp -= size_element;
	SWAP(lp, l, temp, size_element);
	SWAP(rp, r, temp, size_element);

	k = l = lp + size_element;
	r = rp - size_element;

	while (k <= r) {
		if (compare(k, lp) < 0) {
			SWAP(k, l, temp, size_element);
			l += size_element;
		}
		else {
			if (compare(k, rp) > 0) {
				while (compare(r, rp) > 0 && k < r)
					r -= size_element;

				SWAP(k, r, temp, size_element);
				r -= size_element;

				if (compare(k, lp) < 0) {
					SWAP(k, l, temp, size_element);
					l += size_element;
				}
			}
		}
		k += size_element;
	}

	l -= size_element;
	r += size_element;

	SWAP(lp, l, temp, size_element);
	SWAP(rp, r, temp, size_element);

	return (Pair){ .first = (l - arr) / size_element, .second = (r - arr) / size_element };
}

static void dualPivotSortCore(char *arr, char *temp, size_t left, size_t right, size_t size_element, compareFunc compare) {
	if (right > left) {
		if (right - left > THRESHOLD) {
			Pair pair = partition(arr, temp, left, right, size_element, compare);
			size_t i = pair.first, j = pair.second;
			for (; compare(arr + i * size_element, arr + (i + 1) * size_element) == 0 && i < j; ++i);
			for (; compare(arr + j * size_element, arr + (j - 1) * size_element) == 0 && i < j; --j);
			dualPivotSortCore(arr, temp, left, pair.first - 1, size_element, compare);
			dualPivotSortCore(arr, temp, i + 1, j - 1, size_element, compare);
			dualPivotSortCore(arr, temp, pair.second + 1, right, size_element, compare);
		}
		else {
			insertSort(arr, temp, left, right, size_element, compare);
		}
	}
}

void dualPivotQuickSort(void *arr, size_t num_elements, size_t size_element, compareFunc compare) {
	char *temp;

	if (num_elements > 1) {
		temp = malloc(THRESHOLD * size_element);
		if (temp == NULL)
			exit(EXIT_FAILURE);
		dualPivotSortCore(arr, temp, 0, num_elements - 1, size_element, compare);
		free(temp);
	}
}
