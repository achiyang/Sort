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
		sorted_arr[i] = i * (INT_MAX / ARRAY_SIZE);
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
	double rand_avg = 0, sorted_avg = 0;

	initSortedArray();

	srand(time(NULL));
	while (t++ < REPEAT) {
		initRandomArray();

		start = clock();
		mergeSort(rand_arr, ARRAY_SIZE, sizeof(int), compare);
		end = clock();
		rand_avg = (rand_avg * (t - 1) + (end - start)) / t;

		if (isSorted(rand_arr)) {
			printf("random_array: %d\n", end - start);
		}
		else {
			printf("random_array: failed\n");
		}

		start = clock();
		mergeSort(sorted_arr, ARRAY_SIZE, sizeof(int), compare);
		end = clock();
		sorted_avg = (sorted_avg * (t - 1) + (end - start)) / t;

		if (isSorted(sorted_arr)) {
			printf("sorted_array: %d\n", end - start);
		}
		else {
			printf("sorted_array: failed\n");
			initSortedArray();
		}

		printf("\n");
	}

	printf("random_array average: %lf\n", rand_avg);
	printf("sorted_array average: %lf\n", sorted_avg);

	return 0;
}
