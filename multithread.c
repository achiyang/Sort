#include <Windows.h>
#include "sortutils.h"
#include "multithread.h"

#define THRESHOLD 64

typedef struct Params {
	char *arr;
	size_t left;
	size_t right;
	size_t size_element;
	compareFunc compare;
} Params;

static DWORD WINAPI sortCore(LPVOID lpParam) {
	Params *params = (Params *)lpParam;

	if (params->right > params->left) {
		if (params->right - params->left > THRESHOLD) {
			HANDLE hThreadArray[2];
			DWORD dwThreadIdArray[2];
			Params paramsArray[2];
			size_t mid = (params->left + params->right) / 2;

			paramsArray[0] = (Params){
				.arr = params->arr,
				.left = params->left,
				.right = mid,
				.size_element = params->size_element,
				.compare = params->compare
			};
			paramsArray[1] = (Params){
				.arr = params->arr,
				.left = mid + 1,
				.right = params->right,
				.size_element = params->size_element,
				.compare = params->compare
			};

			for (int i = 0; i < 2; i++) {
				hThreadArray[i] = CreateThread(NULL, 0, sortCore, &paramsArray[i], 0, &dwThreadIdArray[i]);
				if (hThreadArray[i] == NULL) {
					ExitProcess(3);
				}
			}

			WaitForMultipleObjects(2, hThreadArray, TRUE, INFINITE);

			for (int i = 0; i < 2; i++) {
				CloseHandle(hThreadArray[i]);
			}

			char *temp = (char *)HeapAlloc(GetProcessHeap(), 0, ((params->right - params->left + 1) >> 1) * params->size_element);
			merge(params->arr, temp, params->left, mid, params->right, params->size_element, params->compare);
			HeapFree(GetProcessHeap(), 0, temp);
		}
		else {
			char *temp = (char *)HeapAlloc(GetProcessHeap(), 0, (params->right - params->left) * params->size_element);
			insertSort(params->arr, temp, params->left, params->right, params->size_element, params->compare);
			HeapFree(GetProcessHeap(), 0, temp);
		}
	}

	return 0;
}

void multiThreadSort(void *arr, size_t num_elements, size_t size_element, compareFunc compare) {
	if (num_elements > 1) {
		Params params = {
			.arr = (char *)arr,
			.left = 0,
			.right = num_elements - 1,
			.size_element = size_element,
			.compare = compare
		};
		sortCore(&params);
	}
}
