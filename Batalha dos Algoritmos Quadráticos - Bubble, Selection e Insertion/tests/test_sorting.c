#include "sorting.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

static void check_one(SortFunction sort, const int *input, size_t n) {
    int original[32];
    int actual[32];
    assert(n <= 32);
    memcpy(original, input, n * sizeof(int));
    memcpy(actual, input, n * sizeof(int));
    SortStats stats = {0, 0};
    sort(actual, n, &stats);
    assert(is_sorted(actual, n));
    assert(same_multiset(original, actual, n));
}

int main(void) {
    const int cases[][8] = {
        {5, 4, 3, 2, 1, 0, -1, -2},
        {1, 2, 3, 4, 5, 6, 7, 8},
        {4, 4, 4, 4, 4, 4, 4, 4},
        {3, -1, 3, 0, -1, 9, 2, 2}
    };
    SortFunction sorts[] = {bubble_sort, selection_sort, insertion_sort};

    for (size_t s = 0; s < sizeof(sorts) / sizeof(sorts[0]); ++s) {
        for (size_t c = 0; c < sizeof(cases) / sizeof(cases[0]); ++c) {
            check_one(sorts[s], cases[c], 8);
        }
        int empty[1] = {0};
        SortStats stats = {0, 0};
        sorts[s](empty, 0, &stats);
        sorts[s](NULL, 0, &stats);
    }

    /* Verifica também a convenção de contagem das métricas. */
    {
        int a[] = {2, 1};
        SortStats bubble = {0, 0};
        SortStats selection = {0, 0};
        SortStats insertion = {0, 0};
        bubble_sort(a, 2, &bubble);
        int b[] = {2, 1};
        selection_sort(b, 2, &selection);
        int c[] = {2, 1};
        insertion_sort(c, 2, &insertion);
        assert(is_sorted(a, 2) && is_sorted(b, 2) && is_sorted(c, 2));
        assert(bubble.comparisons == 1 && bubble.movements == 3);
        assert(selection.comparisons == 1 && selection.movements == 3);
        assert(insertion.comparisons == 1 && insertion.movements == 2);
    }

    puts("Todos os testes passaram.");
    return 0;
}
