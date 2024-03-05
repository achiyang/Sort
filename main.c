#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "sorttest.h"
#include "bubblesort.h"
#include "selectionsort.h"
#include "insertionsort.h"
#include "timsort.h"
#include "mergesort.h"
#include "quicksort.h"
#include "dualpivot.h"
#include "radixsort.h"
#include "treesort.h"
#include "heapsort.h"

/* sortFunc 구조가 아닌 intRadixSort를 sortFunc으로 만들기 위해 */
static void radixSort(void *arr, size_t num_elements, size_t size_element, compareFunc compare) {
	intRadixSort((int *)arr, num_elements, 1626); // n ^ 3 >= UINT_MAX 인 가장 작은 n이 1626
}

static int compareFloat(const void *a, const void *b) {
	float f1 = *(const float *)a;
	float f2 = *(const float *)b;

	if (f1 < f2) return -1;
	if (f1 > f2) return 1;
	return 0;
}

static void initFloatRandom(void *arr, size_t len) {
	float *array = (float *)arr;

	for (size_t i = 0; i < len; i++) {
		array[i] = (float)rand() / RAND_MAX * rand();
	}
}

static int compareInt(const void *a, const void *b) {
	return *(const int *)a - *(const int *)b;
}

static void initIntRandom(void *arr, size_t len) {
	int *array = (int *)arr;

	for (size_t i = 0; i < len; i++) {
		array[i] = INT_MAX / (RAND_MAX * RAND_MAX) * (rand() * rand());
	}
}

static void initIntSorted(void *arr, size_t len) {
	int *array = (int *)arr;

	for (size_t i = 0; i < len; i++) {
		array[i] = (int)(INT_MAX / len * i);
	}
}

static void initIntReversed(void *arr, size_t len) {
	int *array = (int *)arr;

	for (size_t i = 0; i < len; i--) {
		array[i] = (int)(INT_MAX / len * (len - 1 - i));
	}
}

static void initIntSame(void *arr, size_t len) {
	int *array = (int *)arr;

	int value = rand() * rand();
	for (size_t i = 0; i < len; i++) {
		array[i] = value;
	}
}

static void calculateAverage(Test *test) {
	for (int i = 0; i < test->sorts_cnt; i++) {
		for (int j = 0; j < test->arrays_cnt; j++) {
			double avg = 0;
			for (int t = 0; t < test->repeat; t++) {
				avg += test->result[i][j][t];
			}
			avg /= test->repeat;

			double variance = 0;
			for (int t = 0; t < test->repeat; t++) {
				variance += pow(avg - test->result[i][j][t], 2);
			}
			if (test->repeat > 2) {
				variance /= (test->repeat - 1);
			}

			double std_error = sqrt(variance) / sqrt(test->repeat);
			printf("%-*s%-*s  %g ± %g\n", SORTNAME_MAXLEN, test->sorts[i].name, ARRAYNAME_MAXLEN, test->arrays[j].name, avg, std_error);
		}
		printf("\n");
	}
}

static void calculateRank(Test *test) {
	int *rank = (int *)malloc(test->sorts_cnt * sizeof(int));
	if (rank == NULL)
		exit(EXIT_FAILURE);

	size_t **result = (size_t **)malloc(test->sorts_cnt * sizeof(size_t *));
	if (result == NULL)
		exit(EXIT_FAILURE);

	for (int i = 0; i < test->sorts_cnt; i++) {
		result[i] = (size_t *)calloc(test->arrays_cnt, sizeof(size_t));
		if (result[i] == NULL)
			exit(EXIT_FAILURE);
	}

	for (int i = 0; i < test->sorts_cnt; i++) {
		for (int j = 0; j < test->arrays_cnt; j++) {
			for (int t = 0; t < test->repeat; t++) {
				result[i][j] += test->result[i][j][t];
			}
		}
	}

	for (int i = 0; i < test->arrays_cnt; i++) {
		for (int j = 0; j < test->sorts_cnt; j++) {
			rank[j] = j;
		}

		for (int j = test->sorts_cnt; j > 1; j--) {
			for (int k = 1; k < j; k++) {
				if (result[rank[k]][i] < result[rank[k - 1]][i]) {
					int temp = rank[k];
					rank[k] = rank[k - 1];
					rank[k - 1] = temp;
				}
			}
		}

		printf("\n%s\n\n", test->arrays[i].name);
		for (int j = 0; j < test->sorts_cnt; j++) {
			printf("%d %-*s%g\n", j + 1, SORTNAME_MAXLEN, test->sorts[rank[j]].name, (double)result[rank[j]][i] / test->repeat);
		}
		printf("\n");
	}

	for (int i = 0; i < test->sorts_cnt; i++) {
		free(result[i]);
	}
	free(result);
	free(rank);
}

int main() {
	Test *test = create_test();

	// add_sort(test, "Bubble sort", bubbleSort);
	// add_sort(test, "Selection sort", selectionSort);
	// add_sort(test, "Insertion sort", insertionSort);
	add_sort(test, "qsort", qsort);
	add_sort(test, "Tim sort", timSort);
	add_sort(test, "Merge sort", mergeSort);
	add_sort(test, "Quick sort", quickSort);
	add_sort(test, "Dual-Pivot", dualPivotQuickSort);
	add_sort(test, "Radix sort", radixSort);
	add_sort(test, "Tree sort", treeSort);
	add_sort(test, "Heap sort", heapSort);

	const size_t arr_len = 5000000;
	add_arr(test, "int random", initIntRandom, arr_len, sizeof(int), compareInt);
	add_arr(test, "int sorted", initIntSorted, arr_len, sizeof(int), compareInt);
	add_arr(test, "int reversed", initIntReversed, arr_len, sizeof(int), compareInt);
	add_arr(test, "int same", initIntSame, arr_len, sizeof(int), compareInt);
	add_arr(test, "float random", initFloatRandom, arr_len, sizeof(float), compareFloat);

	test_run(test, 10);

	calculateAverage(test);
	calculateRank(test);

	free_test(&test);

	return 0;
}
