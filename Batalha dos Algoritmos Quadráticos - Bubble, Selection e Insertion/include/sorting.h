#ifndef SORTING_H
#define SORTING_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint64_t comparisons;
    uint64_t movements;
} SortStats;

typedef void (*SortFunction)(int *array, size_t n, SortStats *stats);

void bubble_sort(int *array, size_t n, SortStats *stats);
void selection_sort(int *array, size_t n, SortStats *stats);
void insertion_sort(int *array, size_t n, SortStats *stats);

bool is_sorted(const int *array, size_t n);
bool same_multiset(const int *a, const int *b, size_t n);

#endif
