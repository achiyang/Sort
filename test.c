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

typedef struct TestData {
	sortFunc sort;
	char *name;
	size_t random_total;
	size_t sorted_total;
	int initialized;
} TestData;

TestData testdata[20];

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
		printf("%s\trandom_array:\t%d\n", name, end - start);
	}
	else {
		printf("%s\trandom_array:\tfailed\n", name);
	}

	return end - start;
}

static clock_t test_sorted(sortFunc sort, char* name) {
	clock_t start, end;

	start = clock();
	sort(sorted_arr, ARRAY_SIZE, sizeof(testtype), compare);
	end = clock();

	if (isSorted(sorted_arr)) {
		printf("%s\tsorted_array:\t%d\n", name, end - start);
	}
	else {
		printf("%s\tsorted_array:\tfailed\n", name);
		initSortedArray();
	}

	return end - start;
}

static void test_add(sortFunc sort, char *name) {
	int i;
	for (i = 0; testdata[i].initialized; ++i);
	testdata[i].sort = sort;
	testdata[i].name = name;
	testdata[i].random_total = 0;
	testdata[i].sorted_total = 0;
	testdata[i].initialized = 1;
}

static void test_run() {
	int t = 0;

	srand((unsigned)time(0));
	initSortedArray();
	while (t++ < REPEAT) {
		for (int i = 0; testdata[i].initialized; ++i) {
			testdata[i].random_total += test_random(testdata[i].sort, testdata[i].name);
			testdata[i].sorted_total += test_sorted(testdata[i].sort, testdata[i].name);
		}
		printf("\n");
	}

	for (int i = 0; testdata[i].initialized; ++i) {
		printf("%s\trandom_array\ttotal:\t%lld,\taverage:\t%lf\n",
			testdata[i].name,
			testdata[i].random_total,
			(double)testdata[i].random_total / REPEAT);
		printf("%s\tsorted_array\ttotal:\t%lld,\taverage:\t%lf\n",
			testdata[i].name,
			testdata[i].sorted_total,
			(double)testdata[i].sorted_total / REPEAT);
		printf("\n");
	}
}

int main() {
	test_add(mergeSort, "Merge sort");
	test_add(timSort, "Tim sort");
	test_add(heapSort, "Heap sort");

	test_run();

	return 0;
}
