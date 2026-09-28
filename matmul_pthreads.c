#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

typedef struct {
    int thread_id;
    int num_threads;
    int N;
    const double *A;
    const double *B;
    double *C;
} ThreadData;

void *worker_matmul(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int N = data->N;
    int t_id = data->thread_id;
    int num_threads = data->num_threads;

    /* Decomposicao por blocos continuos de linhas completas */
    int rows_per_thread = N / num_threads;
    int start_row = t_id * rows_per_thread;
    int end_row = (t_id == num_threads - 1) ? N : start_row + rows_per_thread;

    for (int i = start_row; i < end_row; i++) {
        for (int j = 0; j < N; j++) {
            double sum = 0.0;
            for (int k = 0; k < N; k++) {
                sum += data->A[i * N + k] * data->B[k * N + j];
            }
            data->C[i * N + j] = sum;
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Uso: %s <N> <num_threads>\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);
    int T = atoi(argv[2]);
    size_t total = (size_t)N * N;

    double *A = (double *)malloc(total * sizeof(double));
    double *B = (double *)malloc(total * sizeof(double));
    double *C = (double *)calloc(total, sizeof(double));

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i * N + j] = (double)(i + j);
            B[i * N + j] = (double)(i * j);
        }
    }

    pthread_t threads[T];
    ThreadData tdata[T];

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int t = 0; t < T; t++) {
        tdata[t].thread_id = t;
        tdata[t].num_threads = T;
        tdata[t].N = N;
        tdata[t].A = A;
        tdata[t].B = B;
        tdata[t].C = C;
        pthread_create(&threads[t], NULL, worker_matmul, &tdata[t]);
    }

    for (int t = 0; t < T; t++) {
        pthread_join(threads[t], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    double gflops = (2.0 * N * N * N) / (elapsed * 1e9);

    printf("[Pthreads Padrao] N: %d | Threads: %d | Tempo: %.4f s | GFLOPS: %.2f\n", 
           N, T, elapsed, gflops);

    free(A);
    free(B);
    free(C);
    return 0;
}