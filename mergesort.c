#include "sortutils.h"
#include "mergesort.h"

// #define MERGESORT_NO_OPTIMIZE

#ifdef MERGESORT_NO_OPTIMIZE
static void merge(Array* array, size_t left, size_t mid, size_t right) {
	size_t i, j, k;
	char *temp, *p;

	temp = (char*)malloc(array->element_size * (right - left + 1));
	if (temp == NULL)
		exit(EXIT_FAILURE);

	i = left;
	j = mid + 1;
	k = 0;

	while (i <= mid && j <= right) {
		if (array->compare(&array->arr[i * array->element_size], &array->arr[j * array->element_size]) <= 0)
			p = &array->arr[i++ * array->element_size];
		else
			p = &array->arr[j++ * array->element_size];
		memcpy(&temp[k++ * array->element_size], p, array->element_size);
	}

	p = &temp[k * array->element_size];
	if (i > mid) {
		if (j <= right)
			memcpy(p, &array->arr[j * array->element_size], (right - j + 1) * array->element_size);
	}
	else {
		if (i <= mid)
			memcpy(p, &array->arr[i * array->element_size], (mid - i + 1) * array->element_size);
	}

	memcpy(&array->arr[left * array->element_size], temp, (right - left + 1) * array->element_size);

	free(temp);
}

static void mergeSortCore(Array* array, size_t left, size_t right) {
	size_t mid;

	if (left < right) {
		mid = (left + right) / 2;
		mergeSortCore(array, left, mid);
		mergeSortCore(array, mid + 1, right);
		merge(array, left, mid, right);
	}
}

void mergeSort(void* arr, size_t num_elements, size_t size_element, compareFunc compare) {
	Array array;

	if (num_elements > 1) {
		array.arr = (char*)arr;
		array.compare = compare;
		array.element_size = size_element;

		mergeSortCore(&array, 0, num_elements - 1);
	}
}
#else
#define THRESHOLD 64

static void mergeSortCore(char* arr, char* temp, size_t left, size_t right, size_t size_element, compareFunc compare) {
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

void mergeSort(void* arr, size_t num_elements, size_t size_element, compareFunc compare) {
	char* temp;

	if (num_elements > 1) {
		temp = (char*)malloc(size_element * MAX(num_elements >> 1, THRESHOLD));
		if (temp == NULL)
			exit(EXIT_FAILURE);

		mergeSortCore((char *)arr, temp, 0, num_elements - 1, size_element, compare);

		free(temp);
	}
}
#endif // MERGESORT_NO_OPTIMIZE
