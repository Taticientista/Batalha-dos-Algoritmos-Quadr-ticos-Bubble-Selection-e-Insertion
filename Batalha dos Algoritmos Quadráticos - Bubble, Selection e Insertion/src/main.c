#include "sorting.h"

#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    const char *name;
    SortFunction function;
} Algorithm;

static const Algorithm algorithms[] = {
    {"Bubble", bubble_sort},
    {"Selection", selection_sort},
    {"Insertion", insertion_sort}
};

typedef struct {
    size_t n;
    size_t repetitions;
    uint32_t seed;
    const char *output;
    bool random_only;
} Config;

static uint32_t rng_next(uint32_t *state) {
    /* xorshift32: determinístico para fins de experimento educacional. */
    uint32_t x = *state;
    if (x == 0) x = UINT32_C(0xA341316C);
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    *state = x;
    return x;
}

static void fill_random(int *a, size_t n, uint32_t seed) {
    uint32_t state = seed ? seed : UINT32_C(0xA341316C);
    for (size_t i = 0; i < n; ++i) {
        a[i] = (int)(rng_next(&state) % 100000U);
    }
}

static void fill_scenario(int *a, size_t n, uint32_t seed, const char *scenario) {
    fill_random(a, n, seed);
    if (strcmp(scenario, "sorted") == 0) {
        for (size_t i = 0; i < n; ++i) a[i] = (int)i;
    } else if (strcmp(scenario, "reversed") == 0) {
        for (size_t i = 0; i < n; ++i) a[i] = (int)(n - i);
    } else if (strcmp(scenario, "duplicates") == 0) {
        /* Estado local mutável: gera valores variados, mas reproduzíveis. */
        uint32_t state = seed ? seed : UINT32_C(0xA341316C);
        for (size_t i = 0; i < n; ++i) a[i] = (int)(rng_next(&state) % 16U);
    }
}

static bool parse_size(const char *s, size_t *out) {
    char *end = NULL;
    errno = 0;
    unsigned long long value = strtoull(s, &end, 10);
    if (errno || !s[0] || !end || *end != '\0' || value == 0 ||
        value > (unsigned long long)SIZE_MAX) return false;
    *out = (size_t)value;
    return true;
}

static bool parse_seed(const char *s, uint32_t *out) {
    size_t value;
    if (!parse_size(s, &value) || value > UINT32_MAX) return false;
    *out = (uint32_t)value;
    return true;
}

static void usage(const char *program) {
    fprintf(stderr,
        "Uso: %s [--n N] [--repetitions R] [--seed S] [--output arquivo.csv] [--random-only]\n",
        program);
}

static bool parse_args(int argc, char **argv, Config *cfg) {
    *cfg = (Config){5000, 7, 20260901U, "results/benchmark.csv", false};
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--random-only") == 0) {
            cfg->random_only = true;
        } else if (strcmp(argv[i], "--n") == 0 && i + 1 < argc) {
            if (!parse_size(argv[++i], &cfg->n)) return false;
        } else if (strcmp(argv[i], "--repetitions") == 0 && i + 1 < argc) {
            if (!parse_size(argv[++i], &cfg->repetitions)) return false;
        } else if (strcmp(argv[i], "--seed") == 0 && i + 1 < argc) {
            if (!parse_seed(argv[++i], &cfg->seed)) return false;
        } else if (strcmp(argv[i], "--output") == 0 && i + 1 < argc) {
            cfg->output = argv[++i];
        } else {
            return false;
        }
    }
    return true;
}

static bool run_one(FILE *out, const char *scenario, const int *base,
                    size_t n, size_t repetition, uint32_t seed,
                    const Algorithm *algorithm) {
    int *work = malloc(n * sizeof(*work));
    if (!work) {
        fprintf(stderr, "Erro: sem memória para vetor de trabalho.\n");
        return false;
    }
    memcpy(work, base, n * sizeof(*work));

    SortStats stats = {0, 0};
    clock_t start = clock();
    algorithm->function(work, n, &stats);
    clock_t finish = clock();

    bool sorted = is_sorted(work, n);
    bool permutation = same_multiset(base, work, n);
    double seconds = (start == (clock_t)-1 || finish == (clock_t)-1)
        ? -1.0 : (double)(finish - start) / (double)CLOCKS_PER_SEC;

    if (fprintf(out, "%s,%s,%zu,%zu,%" PRIu32 ",%" PRIu64 ",%" PRIu64
                ",%.9f,%d,%d\n",
                scenario, algorithm->name, n, repetition, seed,
                stats.comparisons, stats.movements, seconds,
                sorted ? 1 : 0, permutation ? 1 : 0) < 0) {
        free(work);
        return false;
    }

    printf("%-10s | %-9s | n=%-6zu | comparações=%-12" PRIu64
           " | movimentações=%-12" PRIu64 " | tempo=%.6fs | válido=%s\n",
           scenario, algorithm->name, n, stats.comparisons, stats.movements,
           seconds, (sorted && permutation) ? "sim" : "NÃO");

    free(work);
    return sorted && permutation;
}

int main(int argc, char **argv) {
    Config cfg;
    if (!parse_args(argc, argv, &cfg)) {
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    if (cfg.n > SIZE_MAX / sizeof(int)) {
        fprintf(stderr, "Erro: tamanho solicitado excede o limite de memória.\n");
        return EXIT_FAILURE;
    }

    int *base = malloc(cfg.n * sizeof(*base));
    if (!base) {
        fprintf(stderr, "Erro: não foi possível alocar o vetor de tamanho %zu.\n", cfg.n);
        return EXIT_FAILURE;
    }

    FILE *out = fopen(cfg.output, "w");
    if (!out) {
        fprintf(stderr, "Erro ao abrir '%s'. Crie a pasta de destino ou informe --output.\n",
                cfg.output);
        free(base);
        return EXIT_FAILURE;
    }

    fprintf(out, "scenario,algorithm,n,repetition,seed,comparisons,movements,seconds,sorted,permutation_preserved\n");
    const char *scenarios_all[] = {"random", "sorted", "reversed", "duplicates"};
    size_t scenario_count = cfg.random_only ? 1 : 4;
    bool all_valid = true;

    for (size_t s = 0; s < scenario_count; ++s) {
        const char *scenario = scenarios_all[s];
        for (size_t rep = 1; rep <= cfg.repetitions; ++rep) {
            uint32_t scenario_seed = cfg.seed + (uint32_t)(s * 7919U);
            fill_scenario(base, cfg.n, scenario_seed, scenario);

            /* Rotação da ordem para reduzir viés sistemático de aquecimento. */
            for (size_t k = 0; k < 3; ++k) {
                size_t index = (k + rep - 1) % 3;
                if (!run_one(out, scenario, base, cfg.n, rep, scenario_seed,
                             &algorithms[index])) {
                    all_valid = false;
                }
            }
        }
    }

    if (fclose(out) != 0) all_valid = false;
    free(base);

    printf("\nCSV gravado em: %s\n", cfg.output);
    if (!all_valid) {
        fprintf(stderr, "Falha: pelo menos uma saída não passou na validação ou ocorreu erro de escrita.\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
