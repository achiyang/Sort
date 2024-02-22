#include "sortutils.h"
#include "timsort.h"

#define THRESHOLD 64
#define MIN_STACK_SIZE 16

typedef struct Run {
	size_t left;
	size_t right;
} Run;

typedef struct RunStack {
	Run *stack;
	int size;
	int top;
} RunStack;

static void initRunStack(RunStack *stack) {
	stack->stack = malloc(sizeof(Run) * MIN_STACK_SIZE);
	if (stack->stack == NULL)
		exit(EXIT_FAILURE);
	stack->size = MIN_STACK_SIZE;
	stack->top = -1;
}

static void resizeRunStack(RunStack *stack, int size) {
	Run *temp;

	temp = realloc(stack->stack, sizeof(Run) * size);
	if (temp == NULL)
		exit(EXIT_FAILURE);
	stack->stack = temp;
	stack->size = size;
}

static void pushRunStack(RunStack *stack, Run run) {
	if (++stack->top >= stack->size)
		resizeRunStack(stack, stack->size * 2);

	stack->stack[stack->top] = run;
}

static Run popRunStack(RunStack *stack) {
	return stack->stack[stack->top--];
}

static void insertAsDesSort(char *array, char *temp, size_t left, size_t right, int ascending, size_t size_element, compareFunc compare) {
	size_t i;
	size_t temp_left, temp_right, temp_mid;
	char *a, *b;

	if (ascending) {
		for (i = left + 1; i <= right; ++i) {
			a = array + i * size_element;
			if (compare(a, a - size_element) < 0) {
				temp_left = left;
				temp_right = i - 1;
				while (temp_left < temp_right) {
					temp_mid = (temp_left + temp_right) >> 1;
					if (compare(a, array + temp_mid * size_element) < 0) {
						temp_right = temp_mid;
					}
					else {
						temp_left = temp_mid + 1;
					}
				}
				b = array + temp_left * size_element;
				memcpy(temp, b, a - b);
				memcpy(b, a, size_element);
				memcpy(b + size_element, temp, a - b);
			}
		}
	}
	else {
		for (i = left + 1; i <= right; ++i) {
			a = array + i * size_element;
			if (compare(a, a - size_element) >= 0) {
				temp_left = left;
				temp_right = i - 1;
				while (temp_left < temp_right) {
					temp_mid = (temp_left + temp_right) >> 1;
					if (compare(a, array + temp_mid * size_element) >= 0) {
						temp_right = temp_mid;
					}
					else {
						temp_left = temp_mid + 1;
					}
				}
				b = array + temp_left * size_element;
				memcpy(temp, b, a - b);
				memcpy(b, a, size_element);
				memcpy(b + size_element, temp, a - b);
			}
		}
	}
}

static void reverse(char *arr, char *temp, Run run, size_t size_element) {
	char *a, *b;

	a = arr + (run.left * size_element);
	b = arr + (run.right * size_element);
	while (a < b) {
		SWAP(a, b, temp, size_element);
		a += size_element;
		b -= size_element;
	}
}

static Run mergeRun(char *arr, char *temp, Run left, Run right, size_t size_element, compareFunc compare) {
	merge(arr, temp, left.left, left.right, right.right, size_element, compare);
	return (Run) { .left = left.left, .right = right.right };
}

void timSort(void *arr, size_t num_elements, size_t size_element, compareFunc compare) {
	RunStack stack;
	Run run;
	Run r1, r2, r3, r4;
	size_t min_run_size;
	char *array;
	char *temp;
	int boolean;

	const size_t limit = num_elements - 1;
	if (limit > 0) {
		initRunStack(&stack);
		array = (char *)arr;
		min_run_size = num_elements;
		while (min_run_size > THRESHOLD) {
			min_run_size = (min_run_size >> 1) + (min_run_size & 1);
		}
		temp = malloc(size_element * MAX(num_elements >> 1, min_run_size));

		run.left = 0;
		run.right = MIN(min_run_size - 1, limit);
		while (1) {
			if (run.left < limit) {
				boolean = compare(array + run.left * size_element, array + (run.left + 1) * size_element) <= 0;
				insertAsDesSort(array, temp, run.left, run.right, boolean, size_element, compare);

				if (boolean) {
					while (run.right < limit && compare(array + run.right * size_element, array + (run.right + 1) * size_element) <= 0) {
						++run.right;
					}
				}
				else {
					while (run.right < limit && compare(array + run.right * size_element, array + (run.right + 1) * size_element) > 0) {
						++run.right;
					}
				}

				if (!boolean) {
					reverse(array, temp, run, size_element);
				}
			}
			pushRunStack(&stack, run);

			while (stack.top >= 1) {
				r1 = stack.stack[stack.top];
				r2 = stack.stack[stack.top - 1];
				boolean = 0;
				if (stack.top >= 2) {
					r3 = stack.stack[stack.top - 2];
					boolean = r3.right - r3.left <= r1.right - r2.left;
				}
				if (stack.top >= 3 && !boolean) {
					r4 = stack.stack[stack.top - 3];
					boolean = r4.right - r4.left <= r2.right - r3.left;
				}

				if (boolean) {
					--stack.top;
					if (r3.right - r3.left < r1.right - r1.left) {
						stack.stack[stack.top - 1] = mergeRun(array, temp, r3, r2, size_element, compare);
						stack.stack[stack.top] = r1;
					}
					else {
						stack.stack[stack.top] = mergeRun(array, temp, r2, r1, size_element, compare);
					}
				}
				else if (r2.right - r2.left <= r1.right - r1.left) {
					stack.stack[--stack.top] = mergeRun(array, temp, r2, r1, size_element, compare);
				}
				else {
					break;
				}
			}

			if (run.right < limit) {
				run.left = run.right + 1;
				run.right = MIN(run.right + min_run_size, limit);
			}
			else {
				break;
			}
		}

		run = popRunStack(&stack);
		while (stack.top >= 0) {
			run = mergeRun(array, temp, popRunStack(&stack), run, size_element, compare);
		}

		free(stack.stack);
		free(temp);
	}
}
