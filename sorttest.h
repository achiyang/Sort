#ifndef SORTTEST_H
#define SORTTEST_H

#include <time.h>
#include "sorttypes.h"

#define SORTNAME_MAXLEN 18
#define ARRAYNAME_MAXLEN 20

typedef void (*sortFunc)(void *arr, size_t num_elements, size_t size_element, compareFunc compare);

typedef void (*initArrayFunc)(void *arr, size_t len);

struct SortData {
	char *name;
	sortFunc sort;
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

	clock_t ***result; // result[sort][array][repeat]

	int repeat;
} Test;

Test *create_test();

void free_test(Test **testRef);

void add_sort(Test *test, char *name, sortFunc sort);

void add_arr(Test *test, char *name, initArrayFunc initArray, size_t num_elements, size_t size_element, compareFunc compare);

void test_run(Test *test, int repeat);

#endif // SORTTEST_H
