#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MIN(a,b) (((a)<(b))?(a):(b))

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Uso: %s <N> <B>\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);
    int B = atoi(argv[2]);
    size_t total = (size_t)N * N;

    double *A = (double *)malloc(total * sizeof(double));
    double *B_mat = (double *)malloc(total * sizeof(double));
    double *C = (double *)calloc(total, sizeof(double));

    if (!A || !B_mat || !C) {
        perror("Falha na alocacao");
        return 1;
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i * N + j] = (double)(i + j);
            B_mat[i * N + j] = (double)(i * j);
        }
    }

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    /* Multiplicacao com Tiling e ordenacao i-k-j para acesso sequencial */
    for (int ii = 0; ii < N; ii += B) {
        int i_max = MIN(ii + B, N);
        for (int kk = 0; kk < N; kk += B) {
            int k_max = MIN(kk + B, N);
            for (int jj = 0; jj < N; jj += B) {
                int j_max = MIN(jj + B, N);
                
                for (int i = ii; i < i_max; i++) {
                    for (int k = kk; k < k_max; k++) {
                        double r = A[i * N + k];
                        for (int j = jj; j < j_max; j++) {
                            C[i * N + j] += r * B_mat[k * N + j];
                        }
                    }
                }
            }
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    double gflops = (2.0 * N * N * N) / (elapsed * 1e9);

    printf("[Matmul Blocado] N: %d | Bloco B: %d | Tempo: %.4f s | GFLOPS: %.2f | C[0]=%.1f\n", 
           N, B, elapsed, gflops, C[0]);

    free(A);
    free(B_mat);
    free(C);
    return 0;
}