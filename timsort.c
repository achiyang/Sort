#include <stdlib.h>
#include <string.h>
#include "timsort.h"

#define THRESHOLD 32
#define MIN_STACK_SIZE 16
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define SWAP(a, b, temp, size) \
	do { \
		memcpy(temp, a, size); \
		memcpy(a, b, size); \
		memcpy(b, temp, size); \
	} while(0)

typedef struct Run {
	int left;
	int right;
} Run;

typedef struct Stack {
	Run *stack;
	int size;
	int top;
} Stack;

static void initStack(Stack *stack) {
	stack->stack = malloc(sizeof(Run) * MIN_STACK_SIZE);
	if (stack->stack == NULL)
		exit(EXIT_FAILURE);
	stack->size = MIN_STACK_SIZE;
	stack->top = -1;
}

static void resize(Stack *stack, int size) {
	Run *temp;

	temp = realloc(stack->stack, sizeof(Run) * size);
	if (temp == NULL)
		exit(EXIT_FAILURE);
	stack->stack = temp;
	stack->size = size;
}

static void push(Stack *stack, Run run) {
	if (++stack->top >= stack->size)
		resize(stack, stack->size * 2);

	stack->stack[stack->top] = run;
}

static Run pop(Stack *stack) {
	Run ret = stack->stack[stack->top--];

	if (stack->top < stack->size / 4 && stack->size > MIN_STACK_SIZE)
		resize(stack, stack->size / 2);

	return ret;
}

static void insertSort(char *array, char *temp, int left, int right, int ascending, size_t size_element, compareFunc compare) {
	char *a, *b;
	char *a_limit, *b_limit;

	a = array + (left * size_element);
	a_limit = array + (right * size_element);
	b_limit = a;
	if (ascending) {
		for (; a <= a_limit; a += size_element) {
			for (b = a - size_element; b >= b_limit && compare(a, b) < 0; b -= size_element);
			b += size_element;
			if (b < a) {
				memcpy(temp, b, a - b);
				memcpy(b, a, size_element);
				memcpy(b + size_element, temp, a - b);
			}
		}
	}
	else {
		for (; a <= a_limit; a += size_element) {
			for (b = a - size_element; b >= b_limit && compare(a, b) >= 0; b -= size_element);
			b += size_element;
			if (b < a) {
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

static Run merge(char *arr, char *temp, Run left, Run right, size_t size_element, compareFunc compare) {
	Run ret;
	int new_left, new_right;
	int temp_left, temp_mid, temp_right;
	char *data;
	char *a, *b, *c;
	char *a_limit, *b_limit;
	int comp;

	ret.left = left.left;
	ret.right = right.right;
	data = arr + (left.right * size_element);
	if (compare(data, data + size_element) > 0) {
		temp_left = left.left;
		temp_right = left.right;
		data = arr + (right.left * size_element);
		while (temp_left < temp_right) {
			temp_mid = (temp_left + temp_right) / 2;
			if (compare(data, arr + (temp_mid * size_element)) < 0)
				temp_right = temp_mid;
			else
				temp_left = temp_mid + 1;
		}
		new_left = temp_left;

		temp_left = right.left;
		temp_right = right.right;
		data = arr + (left.right * size_element);
		while (temp_left < temp_right) {
			temp_mid = (temp_left + temp_right + 1) / 2;
			if (compare(data, arr + (temp_mid * size_element)) > 0)
				temp_left = temp_mid;
			else
				temp_right = temp_mid - 1;
		}
		new_right = temp_right;

		if (left.right - new_left <= new_right - right.left) {
			a = temp;
			b = arr + (right.left) * size_element;
			c = arr + (new_left * size_element);
			a_limit = temp + (left.right - new_left) * size_element;
			b_limit = arr + (new_right * size_element);

			memcpy(a, c, (left.right - new_left + 1) * size_element);
			comp = compare(a, b);
			while (a <= a_limit && b <= b_limit) {
				if (comp <= 0) {
					data = a + size_element;
					while (data <= a_limit && (comp = compare(data, b)) <= 0) {
						data += size_element;
					}
					memcpy(c, a, data - a);
					c += data - a;
					a = data;
				}
				else {
					data = b + size_element;
					while (data <= b_limit && (comp = compare(a, data)) > 0) {
						data += size_element;
					}
					memmove(c, b, data - b);
					c += data - b;
					b = data;
				}
			}

			if (b > b_limit && a <= a_limit) {
				memcpy(c, a, a_limit - a + size_element);
			}
		}
		else {
			a = temp + (new_right - right.left) * size_element;
			b = arr + (left.right * size_element);
			c = arr + (new_right * size_element);
			a_limit = temp;
			b_limit = arr + (new_left * size_element);

			memcpy(temp, arr + (right.left * size_element), (new_right - right.left + 1) * size_element);
			comp = compare(a, b);
			while (a >= a_limit && b >= b_limit) {
				if (comp >= 0) {
					data = a - size_element;
					while (data >= a_limit && (comp = compare(data, b)) >= 0) {
						data -= size_element;
					}
					c -= a - data;
					memcpy(c + size_element, data + size_element, a - data);
					a = data;
				}
				else {
					data = b - size_element;
					while (data >= b_limit && (comp = compare(a, data)) < 0) {
						data -= size_element;
					}
					c -= b - data;
					memmove(c + size_element, data + size_element, b - data);
					b = data;
				}
			}

			if (b < b_limit && a >= a_limit) {
				memcpy(b_limit, a_limit, a - a_limit + size_element);
			}
		}
	}

	return ret;
}

void timSort(void *arr, size_t num_elements, size_t size_element, compareFunc compare) {
	Stack stack;
	Run run;
	Run r1, r2, r3, r4;
	int min_run_size;
	char *array;
	char *temp;
	int ascending;
	char *p, *limit;

	if (num_elements > 1) {
		initStack(&stack);
		array = (char *)arr;
		min_run_size = num_elements;
		while (min_run_size > THRESHOLD) {
			min_run_size >>= 1;
		}
		temp = malloc(size_element * MAX(num_elements / 2, min_run_size));

		run.left = 0;
		run.right = (int)MIN(min_run_size - 1, num_elements - 1);
		limit = array + (num_elements - 1) * size_element;
		while (1) {
			p = array + (run.left * size_element);
			if (p < limit) {
				ascending = compare(p, p + size_element) <= 0;
				insertSort(array, temp, run.left, run.right, ascending, size_element, compare);

				p = array + (run.right * size_element);
				if (ascending) {
					while (p < limit && compare(p, p + size_element) <= 0) {
						p += size_element;
					}
				}
				else {
					while (p < limit && compare(p, p + size_element) > 0) {
						p += size_element;
					}
				}
				run.right = (int)((p - array) / size_element);

				if (!ascending) {
					reverse(array, temp, run, size_element);
				}
			}
			push(&stack, run);

			while (stack.top >= 1) {
				r1 = stack.stack[stack.top];
				r2 = stack.stack[stack.top - 1];
				ascending = 0;
				if (stack.top >= 2) {
					r3 = stack.stack[stack.top - 2];
					ascending = r3.right - r3.left <= r1.right - r2.left;
				}
				if (!ascending && stack.top >= 3) {
					r4 = stack.stack[stack.top - 3];
					ascending = r4.right - r4.left <= r2.right - r3.left;
				}

				if (ascending) {
					--stack.top;
					if (r3.right - r3.left < r1.right - r1.left) {
						stack.stack[stack.top - 1] = merge(array, temp, r3, r2, size_element, compare);
						stack.stack[stack.top] = r1;
					}
					else {
						stack.stack[stack.top] = merge(array, temp, r2, r1, size_element, compare);
					}
				}
				else if (r2.right - r2.left <= r1.right - r1.left) {
					stack.stack[--stack.top] = merge(array, temp, r2, r1, size_element, compare);
				}
				else {
					break;
				}
			}

			if (p < limit) {
				run.left = run.right + 1;
				run.right = (int)MIN(run.right + min_run_size, num_elements - 1);
			}
			else {
				break;
			}
		}

		run = pop(&stack);
		while (stack.top >= 0) {
			run = merge(array, temp, pop(&stack), run, size_element, compare);
		}

		free(stack.stack);
		free(temp);
	}
}
