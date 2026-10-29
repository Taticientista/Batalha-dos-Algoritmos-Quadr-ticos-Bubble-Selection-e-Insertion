#include "sorting.h"

#include <stdlib.h>

static void swap_int(int *a, int *b, SortStats *stats) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
    stats->movements += 3;
}

void bubble_sort(int *array, size_t n, SortStats *stats) {
    if (!array || !stats || n < 2) return;

    for (size_t end = n; end > 1; --end) {
        bool swapped = false;
        for (size_t j = 1; j < end; ++j) {
            stats->comparisons++;
            if (array[j - 1] > array[j]) {
                swap_int(&array[j - 1], &array[j], stats);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

void selection_sort(int *array, size_t n, SortStats *stats) {
    if (!array || !stats || n < 2) return;

    for (size_t i = 0; i < n - 1; ++i) {
        size_t min_index = i;
        for (size_t j = i + 1; j < n; ++j) {
            stats->comparisons++;
            if (array[j] < array[min_index]) min_index = j;
        }
        if (min_index != i) {
            swap_int(&array[i], &array[min_index], stats);
        }
    }
}

void insertion_sort(int *array, size_t n, SortStats *stats) {
    if (!array || !stats || n < 2) return;

    for (size_t i = 1; i < n; ++i) {
        int key = array[i];
        size_t j = i;

        while (j > 0) {
            stats->comparisons++;
            if (array[j - 1] <= key) break;
            array[j] = array[j - 1];
            stats->movements++;
            --j;
        }
        array[j] = key;
        stats->movements++;
    }
}

bool is_sorted(const int *array, size_t n) {
    if (!array && n != 0) return false;
    for (size_t i = 1; i < n; ++i) {
        if (array[i - 1] > array[i]) return false;
    }
    return true;
}

static int compare_ints(const void *pa, const void *pb) {
    const int a = *(const int *)pa;
    const int b = *(const int *)pb;
    return (a > b) - (a < b);
}

bool same_multiset(const int *a, const int *b, size_t n) {
    if ((!a || !b) && n != 0) return false;
    if (n < 2) return true;

    int *copy_a = malloc(n * sizeof(*copy_a));
    int *copy_b = malloc(n * sizeof(*copy_b));
    if (!copy_a || !copy_b) {
        free(copy_a);
        free(copy_b);
        return false;
    }

    for (size_t i = 0; i < n; ++i) {
        copy_a[i] = a[i];
        copy_b[i] = b[i];
    }
    qsort(copy_a, n, sizeof(*copy_a), compare_ints);
    qsort(copy_b, n, sizeof(*copy_b), compare_ints);

    bool equal = true;
    for (size_t i = 0; i < n; ++i) {
        if (copy_a[i] != copy_b[i]) {
            equal = false;
            break;
        }
    }

    free(copy_a);
    free(copy_b);
    return equal;
}
