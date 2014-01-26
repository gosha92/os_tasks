#include "stdlib.h"
#include "stdio.h"
#include <time.h>

size_t iterations = 3500000;
char* memory;
size_t size;

void test_increment()
{
	for (size_t i = 0; i < iterations; ++i)
		memory[i % size] = 'X';
}

void test_random()
{
	for (size_t i = 0; i < iterations; ++i)
		memory[rand() % size] = 'X';
}

int main(void)
{
    srand((int)time(0));
    
    size = (size_t)3 * 1024 * 1024 * 1024 * 5 / 4;
    memory = (char*)malloc(size);
    
    float start = (float)clock()/CLOCKS_PER_SEC;
    test_increment();
    float end = (float)clock()/CLOCKS_PER_SEC;
    printf("Increment: %0.4f\n", end - start);
    
    start = (float)clock()/CLOCKS_PER_SEC;
    test_random();
    end = (float)clock()/CLOCKS_PER_SEC;
    printf("Random: %0.4f\n", end - start);
}














