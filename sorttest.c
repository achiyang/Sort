#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "sorttest.h"

Test *createTest() {
	Test *test = malloc(sizeof(Test));
	if (test == NULL)
		exit(EXIT_FAILURE);

	test->sorts_cnt = 0;
	test->sorts = NULL;
	test->arrays_cnt = 0;
	test->arrays = NULL;
	test->arr_size = 0;
	test->arr = NULL;

	return test;
}

void freeTest(Test **testRef) {
	Test *test = *testRef;

	if (test != NULL) {
		for (int i = 0; i < test->sorts_cnt; i++) {
			free(test->sorts[i].total_times);
		}
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
	test->sorts[test->sorts_cnt].total_times = NULL;

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

	if (size_element * num_elements > test->arr_size) {
		test->arr_size = size_element * num_elements;
		void *temp = realloc(test->arr, test->arr_size);
		if (temp == NULL)
			exit(EXIT_FAILURE);
		test->arr = temp;
	}

	test->arrays_cnt++;
}

void test_run(Test *test, int repeat) {
	srand((unsigned)time(0));

	int t = repeat;
	for (int i = 0; i < test->sorts_cnt; i++) {
		void *temp = realloc(test->sorts[i].total_times, test->arrays_cnt * sizeof(size_t));
		if (temp == NULL)
			exit(EXIT_FAILURE);
		memset(temp, 0, test->arrays_cnt * sizeof(size_t));
		test->sorts[i].total_times = temp;
	}

	while (t--) {
		for (int i = 0; i < test->sorts_cnt; i++) {
			for (int j = 0; j < test->arrays_cnt; j++) {
				test->arrays[j].initArray(test->arr, test->arrays[j].num_elements);

				clock_t start = clock();
				test->sorts[i].sort(test->arr, test->arrays[j].num_elements, test->arrays[j].size_element, test->arrays[j].compare);
				clock_t end = clock();

				printf("%-18s%-20s%hd\n", test->sorts[i].name, test->arrays[j].name, end - start);
				test->sorts[i].total_times[j] += end - start;
			}
			printf("\n");
		}
		printf("----------------------------------------------\n\n");
	}

	for (int i = 0; i < test->sorts_cnt; i++) {
		for (int j = 0; j < test->arrays_cnt; j++) {
			printf("%-18s%-20savg  %g\n", test->sorts[i].name, test->arrays[j].name, (double)test->sorts[i].total_times[j] / repeat);
		}
		printf("\n");
	}
}
