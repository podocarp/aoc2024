#ifndef AOC
#include <stdbool.h>
#include <stddef.h>

void quicksort_int(int *arr, int len);

void printarr(char *fmt, void *arr, size_t elem_size, size_t elem_len);
void printstr(char *str, int maxlen);

bool strprefix(char *str, char *match, int maxlen);

#define AOC
#endif
