#include "sortutils.h"
#include "radixsort.h"

static int getMax(unsigned int *arr, size_t size) {
	unsigned int max_value = arr[0];

	for (size_t i = 1; i < size; i++) {
		if (arr[i] > max_value) {
			max_value = arr[i];
		}
	}

	return max_value;
}

void intRadixSort(int *arr, size_t size, const size_t radix) {
	if (size > 1) {
		unsigned int *array = (unsigned int *)arr;
		size_t *count;
		int *temp;

		count = malloc(radix * sizeof(size_t));
		if (count == NULL)
			exit(EXIT_FAILURE);
		temp = malloc(size * sizeof(int));
		if (temp == NULL) {
			exit(EXIT_FAILURE);
		}

		unsigned int max_value = getMax(array, size);
		for (size_t exp = 1; max_value > exp; exp *= radix) {
			memset(count, 0, radix * sizeof(size_t));

			for (size_t i = 0; i < size; i++) {
				count[(array[i] / exp) % radix]++;
			}

			for (size_t i = 1; i < radix; i++) {
				count[i] += count[i - 1];
			}

			for (size_t i = size; i > 0; i--) {
				temp[--count[(array[i - 1] / exp) % radix]] = array[i - 1];
			}

			memcpy(array, temp, size * sizeof(int));
		}

		size_t i;
		for (i = size; i > 0; i--) {
			if (array[i - 1] <= INT_MAX) {
				break;
			}
		}
		if (i < size && i > 0) {
			if (i < size / 2) {
				memcpy(temp, array + i, (size - i) * sizeof(int));
				memcpy(array + (size - i), array, i * sizeof(int));
				memcpy(array, temp, (size - i) * sizeof(int));
			}
			else {
				memcpy(temp, array, i * sizeof(int));
				memcpy(array, array + i, (size - i) * sizeof(int));
				memcpy(array + (size - i), temp, i * sizeof(int));
			}
		}

		free(count);
		free(temp);
	}
}
