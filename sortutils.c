#include <string.h>
#include "sortutils.h"

void insertSort(char *arr, char *temp, size_t left, size_t right, size_t size_element, compareFunc compare) {
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

void merge(char *arr, char *temp, size_t left, size_t mid, size_t right, size_t size_element, compareFunc compare) {
	size_t new_left, new_right;
	size_t temp_left, temp_mid, temp_right;
	char *data;
	char *a, *b, *c;
	char *a_limit, *b_limit;
	int comp;

	data = arr + mid * size_element;
	if (compare(data, data + size_element) > 0) {
		temp_left = left;
		temp_right = mid;
		data = arr + (mid + 1) * size_element;
		while (temp_left < temp_right) {
			temp_mid = (temp_left + temp_right) >> 1;
			if (compare(data, arr + temp_mid * size_element) < 0)
				temp_right = temp_mid;
			else
				temp_left = temp_mid + 1;
		}
		new_left = temp_left;

		temp_left = mid + 1;
		temp_right = right;
		data = arr + mid * size_element;
		while (temp_left < temp_right) {
			temp_mid = (temp_left + temp_right + 1) >> 1;
			if (compare(data, arr + temp_mid * size_element) > 0)
				temp_left = temp_mid;
			else
				temp_right = temp_mid - 1;
		}
		new_right = temp_right;

		if (mid - new_left < new_right - mid) {
			a = temp;
			b = arr + (mid + 1) * size_element;
			c = arr + new_left * size_element;
			a_limit = temp + (mid - new_left) * size_element;
			b_limit = arr + new_right * size_element;

			memcpy(a, c, (mid - new_left + 1) * size_element);
			comp = compare(a, b);
			while (a <= a_limit && b <= b_limit) {
				if (comp <= 0) {
					data = a + size_element;
					while (data <= a_limit && (comp = compare(data, b)) <= 0) {
						data += size_element;
					}
					memcpy(c, a, data - a);
					c += data - a;
					a = data;
				}
				else {
					data = b + size_element;
					while (data <= b_limit && (comp = compare(a, data)) > 0) {
						data += size_element;
					}
					memmove(c, b, data - b);
					c += data - b;
					b = data;
				}
			}

			if (b > b_limit && a <= a_limit) {
				memcpy(c, a, a_limit - a + size_element);
			}
		}
		else {
			a = temp + (new_right - mid - 1) * size_element;
			b = arr + mid * size_element;
			c = arr + new_right * size_element;
			a_limit = temp;
			b_limit = arr + new_left * size_element;

			memcpy(temp, arr + (mid + 1) * size_element, (new_right - mid) * size_element);
			comp = compare(a, b);
			while (a >= a_limit && b >= b_limit) {
				if (comp >= 0) {
					data = a - size_element;
					while (data >= a_limit && (comp = compare(data, b)) >= 0) {
						data -= size_element;
					}
					c -= a - data;
					memcpy(c + size_element, data + size_element, a - data);
					a = data;
				}
				else {
					data = b - size_element;
					while (data >= b_limit && (comp = compare(a, data)) < 0) {
						data -= size_element;
					}
					c -= b - data;
					memmove(c + size_element, data + size_element, b - data);
					b = data;
				}
			}

			if (b < b_limit && a >= a_limit) {
				memcpy(b_limit, a_limit, a - a_limit + size_element);
			}
		}
	}
}
