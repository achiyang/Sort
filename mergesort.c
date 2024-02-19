#include <stdlib.h>
#include <string.h>
#include "mergesort.h"

typedef struct Array {
	char* arr;
	size_t element_size;
	compareFunc compare;
} Array;

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
#define THRESHOLD 20
#define MAX(a, b) ((a) > (b) ? (a) : (b))

static void insertSort(Array* array, void* temp, size_t left, size_t right) {
	size_t i;
	size_t temp_left, temp_right, temp_mid;
	char *a, *b;

	for (i = left + 1; i <= right; ++i) {
		a = array->arr + i * array->element_size;
		if (array->compare(a, a - array->element_size) < 0) {
			temp_left = left;
			temp_right = i - 1;
			while (temp_left < temp_right) {
				temp_mid = (temp_left + temp_right) / 2;
				if (array->compare(a, array->arr + temp_mid * array->element_size) < 0) {
					temp_right = temp_mid;
				}
				else {
					temp_left = temp_mid + 1;
				}
			}
			b = array->arr + temp_left * array->element_size;
			memcpy(temp, b, a - b);
			memcpy(b, a, array->element_size);
			memcpy(b + array->element_size, temp, a - b);
		}
	}
}

static void merge(Array* array, char* temp, size_t left, size_t mid, size_t right) {
	size_t new_left, new_right;
	size_t temp_left, temp_mid, temp_right;
	char *data;
	char *a, *b, *c;
	char *a_limit, *b_limit;
	int comp;

	data = array->arr + (mid * array->element_size);
	if (array->compare(data, data + array->element_size) > 0) {
		temp_left = left;
		temp_right = mid;
		data = array->arr + (mid + 1) * array->element_size;
		while (temp_left < temp_right) {
			temp_mid = (temp_left + temp_right) / 2;
			if (array->compare(data, array->arr + (temp_mid * array->element_size)) < 0)
				temp_right = temp_mid;
			else
				temp_left = temp_mid + 1;
		}
		new_left = temp_left;

		temp_left = mid + 1;
		temp_right = right;
		data = array->arr + (mid * array->element_size);
		while (temp_left < temp_right) {
			temp_mid = (temp_left + temp_right + 1) / 2;
			if (array->compare(data, array->arr + (temp_mid * array->element_size)) > 0)
				temp_left = temp_mid;
			else
				temp_right = temp_mid - 1;
		}
		new_right = temp_right;

		if (mid - new_left < new_right - mid) {
			a = temp;
			b = array->arr + (mid + 1) * array->element_size;
			c = array->arr + (new_left * array->element_size);
			a_limit = temp + (mid - new_left) * array->element_size;
			b_limit = array->arr + (new_right * array->element_size);

			memcpy(a, c, (mid - new_left + 1) * array->element_size);
			comp = array->compare(a, b);
			while (a <= a_limit && b <= b_limit) {
				if (comp <= 0) {
					data = a + array->element_size;
					while (data <= a_limit && (comp = array->compare(data, b)) <= 0) {
						data += array->element_size;
					}
					memcpy(c, a, data - a);
					c += data - a;
					a = data;
				}
				else {
					data = b + array->element_size;
					while (data <= b_limit && (comp = array->compare(a, data)) > 0) {
						data += array->element_size;
					}
					memmove(c, b, data - b);
					c += data - b;
					b = data;
				}
			}

			if (b > b_limit && a <= a_limit) {
				memcpy(c, a, a_limit - a + array->element_size);
			}
		}
		else {
			a = temp + (new_right - mid - 1) * array->element_size;
			b = array->arr + (mid * array->element_size);
			c = array->arr + (new_right * array->element_size);
			a_limit = temp;
			b_limit = array->arr + (new_left * array->element_size);

			memcpy(temp, array->arr + (mid + 1) * array->element_size, (new_right - mid) * array->element_size);
			comp = array->compare(a, b);
			while (a >= a_limit && b >= b_limit) {
				if (comp >= 0) {
					data = a - array->element_size;
					while (data >= a_limit && (comp = array->compare(data, b)) >= 0) {
						data -= array->element_size;
					}
					c -= a - data;
					memcpy(c + array->element_size, data + array->element_size, a - data);
					a = data;
				}
				else {
					data = b - array->element_size;
					while (data >= b_limit && (comp = array->compare(a, data)) < 0) {
						data -= array->element_size;
					}
					c -= b - data;
					memmove(c + array->element_size, data + array->element_size, b - data);
					b = data;
				}
			}

			if (b < b_limit && a >= a_limit) {
				memcpy(b_limit, a_limit, a - a_limit + array->element_size);
			}
		}
	}
}

static void mergeSortCore(Array* array, char* temp, size_t left, size_t right) {
	size_t mid;

	if (left < right) {
		if (right - left > THRESHOLD) {
			mid = (left + right) / 2;
			mergeSortCore(array, temp, left, mid);
			mergeSortCore(array, temp, mid + 1, right);
			merge(array, temp, left, mid, right);
		}
		else {
			insertSort(array, temp, left, right);
		}
	}
}

void mergeSort(void* arr, size_t num_elements, size_t size_element, compareFunc compare) {
	Array array;
	char* temp;

	if (num_elements > 1) {
		temp = (char*)malloc(size_element * MAX(num_elements / 2, THRESHOLD));
		if (temp == NULL)
			exit(EXIT_FAILURE);

		array.arr = arr;
		array.element_size = size_element;
		array.compare = compare;

		mergeSortCore(&array, temp, 0, num_elements - 1);

		free(temp);
	}
}
#endif // MERGESORT_NO_OPTIMIZE
