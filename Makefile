CC = gcc
CFLAGS_BASE = -Wall -Wextra
PTHREAD = -pthread

all: opt0 opt3

opt0:
	$(CC) $(CFLAGS_BASE) -O0 varredura_linha.c -o varredura_linha_O0
	$(CC) $(CFLAGS_BASE) -O0 varredura_coluna.c -o varredura_coluna_O0
	$(CC) $(CFLAGS_BASE) -O0 matmul_padrao.c -o matmul_padrao_O0
	$(CC) $(CFLAGS_BASE) -O0 matmul_bloco.c -o matmul_bloco_O0
	$(CC) $(CFLAGS_BASE) -O0 $(PTHREAD) matmul_pthreads.c -o matmul_pthreads_O0
	$(CC) $(CFLAGS_BASE) -O0 $(PTHREAD) matmul_pthreads_bloco.c -o matmul_pthreads_bloco_O0

opt3:
	$(CC) $(CFLAGS_BASE) -O3 varredura_linha.c -o varredura_linha_O3
	$(CC) $(CFLAGS_BASE) -O3 varredura_coluna.c -o varredura_coluna_O3
	$(CC) $(CFLAGS_BASE) -O3 matmul_padrao.c -o matmul_padrao_O3
	$(CC) $(CFLAGS_BASE) -O3 matmul_bloco.c -o matmul_bloco_O3
	$(CC) $(CFLAGS_BASE) -O3 $(PTHREAD) matmul_pthreads.c -o matmul_pthreads_O3
	$(CC) $(CFLAGS_BASE) -O3 $(PTHREAD) matmul_pthreads_bloco.c -o matmul_pthreads_bloco_O3

clean:
	rm -f *_O0 *_O3 cachegrind.out.*