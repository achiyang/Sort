#ifndef SORTUTILS_H
#define SORTUTILS_H

#include <stdlib.h>
#include <string.h>
#include "sorttypes.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define SWAP(a, b, temp, size) \
	do { \
		memcpy(temp, a, size); \
		memcpy(a, b, size); \
		memcpy(b, temp, size); \
	} while(0)

void insertSort(char *arr, char *temp, size_t left, size_t right, size_t size_element, compareFunc compare);

void merge(char *arr, char *temp, size_t left, size_t mid, size_t right, size_t size_element, compareFunc compare);

#endif
