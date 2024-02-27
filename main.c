#include <stdio.h>
#include <stdlib.h>
#include "sorttest.h"
#include "timsort.h"
#include "mergesort.h"
#include "quicksort.h"
#include "dualpivot.h"
#include "heapsort.h"
#include "insertionsort.h"
#include "bubblesort.h"
#include "selectionsort.h"
#include "radixsort.h"

/* sortFunc 구조가 아닌 intRadixSort를 sortFunc으로 만들기 위해 */
static void radixSort(void *arr, size_t num_elements, size_t size_element, compareFunc compare) {
	intRadixSort((int *)arr, num_elements, 1626); // 부호 없는 float의 정렬도 가능함
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

static void calculateRank(Test *test) {
	int *rank = malloc(sizeof(int) * test->sorts_cnt);
	if (rank == NULL)
		exit(EXIT_FAILURE);

	for (int i = 0; i < test->arrays_cnt; i++) {
		for (int j = 0; j < test->sorts_cnt; j++) {
			rank[j] = j;
		}

		for (int j = test->sorts_cnt; j > 1; j--) {
			for (int k = 1; k < j; k++) {
				if (test->sorts[rank[k]].total_times[i] < test->sorts[rank[k - 1]].total_times[i]) {
					int temp = rank[k];
					rank[k] = rank[k - 1];
					rank[k - 1] = temp;
				}
			}
		}

		printf("\n%s\n\n", test->arrays[i].name);
		for (int j = 0; j < test->sorts_cnt; j++) {
			printf("%d %-18s%g\n", j + 1, test->sorts[rank[j]].name, (double)test->sorts[rank[j]].total_times[i] / test->repeat);
		}
		printf("\n");
	}

	free(rank);
}

int main() {
	Test *test = create_test();

	// add_sort(test, "Bubble sort", bubbleSort);
	// add_sort(test, "Selection sort", selectionSort);
	// add_sort(test, "Insertion sort", insertionSort);
	add_sort(test, "Radix sort", radixSort);
	add_sort(test, "qsort", qsort);
	add_sort(test, "Tim sort", timSort);
	add_sort(test, "Merge sort", mergeSort);
	add_sort(test, "Quick sort", quickSort);
	add_sort(test, "Dual-Pivot", dualPivotQuickSort);
	add_sort(test, "Heap sort", heapSort);

	const size_t arr_len = 5000000;
	add_arr(test, "int random", initIntRandom, arr_len, sizeof(int), compareInt);
	add_arr(test, "int sorted", initIntSorted, arr_len, sizeof(int), compareInt);
	add_arr(test, "int reversed", initIntReversed, arr_len, sizeof(int), compareInt);
	add_arr(test, "int same", initIntSame, arr_len, sizeof(int), compareInt);
	add_arr(test, "float random", initFloatRandom, arr_len, sizeof(float), compareFloat);

	test_run(test, 10);

	calculateRank(test);

	free_test(&test);

	return 0;
}
