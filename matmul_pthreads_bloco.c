#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MIN(a,b) (((a)<(b))?(a):(b))

typedef struct {
    int thread_id;
    int num_threads;
    int N;
    int B;
    const double *A;
    const double *B_mat;
    double *C;
} ThreadDataBlock;

void *worker_matmul_block(void *arg) {
    ThreadDataBlock *d = (ThreadDataBlock *)arg;
    int N = d->N;
    int B = d->B;
    int t_id = d->thread_id;
    int T = d->num_threads;

    int rows_per_thread = N / T;
    int r_start = t_id * rows_per_thread;
    int r_end = (t_id == T - 1) ? N : r_start + rows_per_thread;

    for (int ii = r_start; ii < r_end; ii += B) {
        int i_max = MIN(ii + B, r_end);
        for (int kk = 0; kk < N; kk += B) {
            int k_max = MIN(kk + B, N);
            for (int jj = 0; jj < N; jj += B) {
                int j_max = MIN(jj + B, N);

                for (int i = ii; i < i_max; i++) {
                    for (int k = kk; k < k_max; k++) {
                        double r = d->A[i * N + k];
                        for (int j = jj; j < j_max; j++) {
                            d->C[i * N + j] += r * d->B_mat[k * N + j];
                        }
                    }
                }
            }
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Uso: %s <N> <num_threads> <B>\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);
    int T = atoi(argv[2]);
    int B = atoi(argv[3]);
    size_t total = (size_t)N * N;

    double *A = (double *)malloc(total * sizeof(double));
    double *B_mat = (double *)malloc(total * sizeof(double));
    double *C = (double *)calloc(total, sizeof(double));

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i * N + j] = (double)(i + j);
            B_mat[i * N + j] = (double)(i * j);
        }
    }

    pthread_t threads[T];
    ThreadDataBlock tdata[T];

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int t = 0; t < T; t++) {
        tdata[t].thread_id = t;
        tdata[t].num_threads = T;
        tdata[t].N = N;
        tdata[t].B = B;
        tdata[t].A = A;
        tdata[t].B_mat = B_mat;
        tdata[t].C = C;
        pthread_create(&threads[t], NULL, worker_matmul_block, &tdata[t]);
    }

    for (int t = 0; t < T; t++) {
        pthread_join(threads[t], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    double gflops = (2.0 * N * N * N) / (elapsed * 1e9);

    printf("[Pthreads Blocado] N: %d | Threads: %d | B: %d | Tempo: %.4f s | GFLOPS: %.2f\n", 
           N, T, B, elapsed, gflops);

    free(A);
    free(B_mat);
    free(C);
    return 0;
}