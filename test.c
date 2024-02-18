#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "mergesort.h"
#include "heapsort.h"
#include "timsort.h"

#define ARRAY_SIZE 10000000
#define REPEAT 10
#define TEST_TYPE int
#define TYPE_MAX INT_MAX

typedef TEST_TYPE testtype;
typedef void (*sortFunc)(void* arr, size_t num_elements, size_t size_element, compareFunc compare);

testtype rand_arr[ARRAY_SIZE];
testtype sorted_arr[ARRAY_SIZE];

static int compare(const void* a, const void* b) {
	return (int)(*((const testtype*)a) - *((const testtype*)b));
}

static void initRandomArray() {
	for (int i = 0; i < ARRAY_SIZE; i++) {
		rand_arr[i] = (testtype)(((double)TYPE_MAX / (RAND_MAX * RAND_MAX)) * (rand() * rand()));
	}
}

static void initSortedArray() {
	for (int i = 0; i < ARRAY_SIZE; i++) {
		sorted_arr[i] = (testtype)(((double)TYPE_MAX / ARRAY_SIZE) * i);
	}
}

static int isSorted(testtype* arr) {
	for (int i = 1; i < ARRAY_SIZE; i++) {
		if (compare(arr + (i - 1), arr + i) > 0) {
			return 0;
		}
	}
	return 1;
}

static clock_t test_random(sortFunc sort, char* name) {
	clock_t start, end;

	initRandomArray();

	start = clock();
	sort(rand_arr, ARRAY_SIZE, sizeof(testtype), compare);
	end = clock();

	if (isSorted(rand_arr)) {
		printf("%s random_array: %d\n", name, end - start);
	}
	else {
		printf("%s random_array: failed\n", name);
	}

	return end - start;
}

static clock_t test_sorted(sortFunc sort, char* name) {
	clock_t start, end;

	start = clock();
	sort(sorted_arr, ARRAY_SIZE, sizeof(testtype), compare);
	end = clock();

	if (isSorted(sorted_arr)) {
		printf("%s sorted_array: %d\n", name, end - start);
	}
	else {
		printf("%s sorted_array: failed\n", name);
		initSortedArray();
	}

	return end - start;
}

int main() {
	int t = 0;

	initSortedArray();
	srand((unsigned)time(0));

	while (t++ < REPEAT) {
		test_random(mergeSort, "Merge sort");
		test_sorted(mergeSort, "Merge sort");

		test_random(timSort, "Tim sort");
		test_sorted(timSort, "Tim sort");

		test_random(heapSort, "Heap sort");
		test_sorted(heapSort, "Heap sort");

		printf("\n");
	}

	return 0;
}
