#include "stdlib.h"
#include "stdio.h"
#include "time.h"
#include "pthread.h"

size_t X[2048][2048];
size_t Y[2048][2048];
size_t result[2048][2048];

size_t A[256][256][8][8];
size_t B[256][256][8][8];
size_t result2[256][256][8][8];

void* multiply(void* id)
{
	size_t thread_id = *(size_t*)id;
	size_t start = 256 * thread_id;
	size_t end = start + 256;

	for (size_t i = start; i < end; ++i) // Stroki
		for (size_t j = 0; j < 2048; ++j) // Stolbcy
			for (size_t k = 0; k < 2048; ++k)
				result[i][j] += X[i][k] * Y[k][j];
}

void multiply_test(void)
{
	pthread_t threads[8];
	for (size_t i = 0; i < 8; ++i)
		pthread_create(&threads[i], NULL, multiply, new size_t(i));
	for (size_t i = 0; i < 8; ++i)
		pthread_join(threads[i], NULL);
}

void* multiply_block(void *id)
{
	size_t thread_id = *(size_t*)id;
	size_t start = 256 * thread_id;
	size_t end = start + 256;


	for (size_t i = start; i < end; ++i)
	{
		size_t block_i = i / 8;
		size_t offset_i = i % 8;

		for (size_t block_j = 0; block_j < 256; ++block_j)
		for (size_t offset_j = 0; offset_j < 8; ++offset_j)
			for (size_t k1 = 0; k1 < 256; ++k1)
			for (size_t k2 = 0; k2 < 8; ++k2)
				result2[block_i][block_j][offset_i][offset_j] += A[block_i][k1][offset_i][k2] * B[k1][block_j][k2][offset_j];
	}
}

void multiply_block_test(void)
{
	pthread_t threads[8];
	for (size_t i = 0; i < 8; ++i)
		pthread_create(&threads[i], NULL, multiply_block, new size_t(i));
	for (size_t i = 0; i < 8; ++i)
		pthread_join(threads[i], NULL);
}

int main(void)
{
	double start = (double)clock()/CLOCKS_PER_SEC; // START
	multiply_test();
	double end = (double)clock()/CLOCKS_PER_SEC;
	printf("Normal: %0.4f\n", end - start); // END

	start = (double)clock()/CLOCKS_PER_SEC; // START
	multiply_block_test();
	end = (double)clock()/CLOCKS_PER_SEC;
	printf("Blocks: %0.4f\n", end - start); // END
}





