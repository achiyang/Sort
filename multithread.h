#ifndef MULTITHREAD_H
#define MULTITHREAD_H

#include "sorttypes.h"

void multiThreadSort(void *arr, size_t num_elements, size_t size_element, compareFunc compare);

#endif // MULTITHREAD_H
