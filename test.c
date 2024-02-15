#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mergesort.h"

#define ARRAY_SIZE 10000000
#define REPEAT 10

int rand_arr[ARRAY_SIZE];
int sorted_arr[ARRAY_SIZE];

static int compare(const void* a, const void* b) {
	return *((int*)a) - *((int*)b);
}

static void initRandomArray() {
	for (int i = 0; i < ARRAY_SIZE; i++) {
		rand_arr[i] = rand() * rand() + i;
	}
}

static void initSortedArray() {
	for (int i = 0; i < ARRAY_SIZE; i++) {
		sorted_arr[i] = i;
	}
}

static int isSorted(int* arr) {
	for (int i = 1; i < ARRAY_SIZE; i++) {
		if (arr[i] < arr[i - 1]) {
			return 0;
		}
	}
	return 1;
}

int main() {
	int t = 0;
	clock_t start, end;

	initSortedArray();

	srand(time(NULL));
	while (t++ < REPEAT) {
		initRandomArray();

		start = clock();
		mergeSort(rand_arr, ARRAY_SIZE, sizeof(int), compare);
		end = clock();

		if (isSorted(rand_arr)) {
			printf("%d: random_array: %d\n", t, end - start);
		}
		else {
			printf("%d: random_array: failed\n", t);
		}

		start = clock();
		mergeSort(sorted_arr, ARRAY_SIZE, sizeof(int), compare);
		end = clock();

		if (isSorted(sorted_arr)) {
			printf("%d: sorted_array: %d\n", t, end - start);
		}
		else {
			printf("%d: sorted_array: failed\n", t);
			initSortedArray();
		}
	}

	return 0;
}
