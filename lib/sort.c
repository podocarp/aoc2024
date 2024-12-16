#include <stdio.h>
#include <stdlib.h>

static void _quicksort_int(int *arr, int left, int right);
static void swap(int *arr, int i, int j);

static void swap(int *arr, int i, int j) {
    int tmp = arr[i];
    arr[i] = arr[j];
    arr[j] = tmp;
}

static void _quicksort_int(int *arr, int left, int right) {
    if (left >= right) {
        return;
    }
    swap(arr, left, (left + right) / 2);
    int end = left;
    for (int i = left + 1; i <= right; i++) {
        if (arr[i] < arr[left]) {
            swap(arr, i, ++end);
        }
    }
    swap(arr, end, left);
    _quicksort_int(arr, left, end - 1);
    _quicksort_int(arr, end + 1, right);
}

void quicksort_int(int *arr, int len) { _quicksort_int(arr, 0, len - 1); }
