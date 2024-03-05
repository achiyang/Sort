#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sorttest.h"

static int isSorted(void *arr, size_t num_elements, size_t size_element, compareFunc compare) {
	char *array = (char *)arr;

	for (size_t i = 1; i < num_elements; i++) {
		if (compare(array + i * size_element, array + (i - 1) * size_element) < 0) {
			return 0;
		}
	}

	return 1;
}

static void freeResult(Test *test) {
	if (test->result == NULL)
		return;

	for (int i = 0; i < test->sorts_cnt; i++) {
		for (int j = 0; j < test->arrays_cnt; j++) {
			free(test->result[i][j]);
		}
		free(test->result[i]);
	}
	free(test->result);

	test->result = NULL;
}

Test *create_test() {
	Test *test = malloc(sizeof(Test));
	if (test == NULL)
		exit(EXIT_FAILURE);

	test->sorts_cnt = 0;
	test->sorts = NULL;
	test->arrays_cnt = 0;
	test->arrays = NULL;
	test->arr_size = 0;
	test->arr = NULL;
	test->result = NULL;
	test->repeat = 0;

	return test;
}

void free_test(Test **testRef) {
	Test *test = *testRef;

	if (test != NULL) {
		freeResult(test);
		free(test->sorts);
		free(test->arrays);
		free(test->arr);
		free(test);

		*testRef = NULL;
	}
}

void add_sort(Test *test, char *name, sortFunc sort) {
	void *temp = realloc(test->sorts, sizeof(struct SortData) * (test->sorts_cnt + 1));
	if (temp == NULL)
		exit(EXIT_FAILURE);
	test->sorts = temp;

	test->sorts[test->sorts_cnt].name = name;
	test->sorts[test->sorts_cnt].sort = sort;

	test->sorts_cnt++;
}

void add_arr(Test *test, char *name, initArrayFunc initArray, size_t num_elements, size_t size_element, compareFunc compare) {
	void *temp = realloc(test->arrays, sizeof(struct ArrayData) * (test->arrays_cnt + 1));
	if (temp == NULL)
		exit(EXIT_FAILURE);
	test->arrays = temp;

	test->arrays[test->arrays_cnt].name = name;
	test->arrays[test->arrays_cnt].initArray = initArray;
	test->arrays[test->arrays_cnt].num_elements = num_elements;
	test->arrays[test->arrays_cnt].size_element = size_element;
	test->arrays[test->arrays_cnt].compare = compare;

	if (size_element * num_elements > test->arr_size)
		test->arr_size = size_element * num_elements;

	test->arrays_cnt++;
}

void test_run(Test *test, int repeat) {
	srand((unsigned)time(0));

	test->repeat = repeat;

	if (test->result != NULL) {
		freeResult(test);
	}

	test->result = (clock_t ***)malloc(test->sorts_cnt * sizeof(clock_t **));
	if (test->result == NULL)
		exit(EXIT_FAILURE);

	for (int i = 0; i < test->sorts_cnt; i++) {
		test->result[i] = (clock_t **)malloc(test->arrays_cnt * sizeof(clock_t *));
		if (test->result[i] == NULL)
			exit(EXIT_FAILURE);

		for (int j = 0; j < test->arrays_cnt; j++) {
			test->result[i][j] = (clock_t *)calloc(test->repeat, sizeof(clock_t));
			if (test->result[i][j] == NULL)
				exit(EXIT_FAILURE);
		}
	}

	void *temp = realloc(test->arr, test->arr_size);
	if (temp == NULL)
		exit(EXIT_FAILURE);
	test->arr = temp;

	for (int t = 0; t < repeat; t++) {
		for (int i = 0; i < test->sorts_cnt; i++) {
			for (int j = 0; j < test->arrays_cnt; j++) {
				test->arrays[j].initArray(test->arr, test->arrays[j].num_elements);

				clock_t start = clock();
				test->sorts[i].sort(test->arr, test->arrays[j].num_elements, test->arrays[j].size_element, test->arrays[j].compare);
				clock_t end = clock();

				if (isSorted(test->arr, test->arrays[j].num_elements, test->arrays[j].size_element, test->arrays[j].compare)) {
					printf("%-*s%-*s%hd\n", SORTNAME_MAXLEN, test->sorts[i].name, ARRAYNAME_MAXLEN, test->arrays[j].name, end - start);
					test->result[i][j][t] = end - start;
				}
				else {
					printf("%-*s%-*sfailed\n", SORTNAME_MAXLEN, test->sorts[i].name, ARRAYNAME_MAXLEN, test->arrays[j].name);
					return;
				}
			}
			printf("\n");
		}
		printf("----------------------------------------------\n\n");
	}
}
