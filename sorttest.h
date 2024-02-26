#ifndef SORTTEST_H
#define SORTTEST_H

#include "sorttypes.h"

typedef void (*sortFunc)(void *arr, size_t num_elements, size_t size_element, compareFunc compare);

typedef void (*initArrayFunc)(void *arr, size_t len);

struct SortData {
	char *name;
	sortFunc sort;
	size_t *total_times;
};

struct ArrayData {
	char *name;
	initArrayFunc initArray;
	size_t num_elements;
	size_t size_element;
	compareFunc compare;
};

typedef struct Test {
	int sorts_cnt;
	struct SortData *sorts;

	int arrays_cnt;
	struct ArrayData *arrays;

	size_t arr_size;
	void *arr;

	int repeat;
} Test;

void add_sort(Test *test, char *name, sortFunc sort);

void add_arr(Test *test, char *name, initArrayFunc initArray, size_t num_elements, size_t size_element, compareFunc compare);

void test_run(Test *test, int repeat);

#endif // SORTTEST_H
