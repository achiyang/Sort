#ifndef MERGESORT_H
#define MERGESORT_H

#include <stdint.h>

typedef int (*compareFunc)(const void*, const void*);

void mergeSort(void* arr, size_t num_elements, size_t size_element, compareFunc compare);

#endif // MERGESORT_H
