#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <N>\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);
    size_t total_elements = (size_t)N * N;
    double *A = (double *)malloc(total_elements * sizeof(double));
    if (!A) {
        perror("Erro ao alocar memoria para A");
        return 1;
    }

    /* Inicializacao deterministica */
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i * N + j] = (double)(i + j);
        }
    }

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    /* Varredura Column-Major (Coluna) */
    long pares = 0;
    for (int j = 0; j < N; j++) {
        for (int i = 0; i < N; i++) {
            if (((int)A[i * N + j]) % 2 == 0) {
                pares++;
            }
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("[Coluna] N: %d | Pares: %ld | Tempo: %.6f s\n", N, pares, elapsed);

    free(A);
    return 0;
}