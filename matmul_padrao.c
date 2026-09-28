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
    size_t total = (size_t)N * N;

    double *A = (double *)malloc(total * sizeof(double));
    double *B = (double *)malloc(total * sizeof(double));
    double *C = (double *)calloc(total, sizeof(double));

    if (!A || !B || !C) {
        perror("Falha na alocacao");
        return 1;
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i * N + j] = (double)(i + j);
            B[i * N + j] = (double)(i * j);
        }
    }

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    /* Multiplicacao canonica i-j-k */
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            double sum = 0.0;
            for (int k = 0; k < N; k++) {
                sum += A[i * N + k] * B[k * N + j];
            }
            C[i * N + j] = sum;
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    double gflops = (2.0 * N * N * N) / (elapsed * 1e9);

    printf("[Matmul Padrao] N: %d | Tempo: %.4f s | GFLOPS: %.2f | C[0]=%.1f\n", 
           N, elapsed, gflops, C[0]);

    free(A);
    free(B);
    free(C);
    return 0;
}