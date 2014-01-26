#include "stdlib.h"
#include "stdio.h"
#include "time.h"
#include "pthread.h"

volatile int counters[150];
volatile int counters_128[150][128];


void* test_counters(void* id)
{
    size_t* i = (size_t*)id;
    for (size_t j = 0; j < 60000; ++j)
        counters[*i] += 1;
}


void* test_counters_128(void* id)
{
    size_t* i = (size_t*)id;
    for (size_t j = 0; j < 60000; ++j)
        counters_128[*i][0] += 1;
}



void testing(void*(*function)(void*))
{
    pthread_t threads[150];
    for (size_t i = 0; i < 150; ++i)
	{
        pthread_create(&threads[i], NULL, function, new size_t(i));
	}
    for (size_t i = 0; i < 150; ++i)
	{
        pthread_join(threads[i], NULL);
	}
}

int main(void)
{
    srand((int)time(0));

    float start = (float)clock()/CLOCKS_PER_SEC;
    testing(test_counters);   
    float end = (float)clock()/CLOCKS_PER_SEC;
    printf("Normal: %0.4f\n", end - start);
    
    start = (float)clock()/CLOCKS_PER_SEC;
    testing(test_counters_128);
    end = (float)clock()/CLOCKS_PER_SEC;
    printf("[128]: %0.4f\n", end - start);
}





//size_t threadNum = *reinterpret_cast<size_t*>(arg);


