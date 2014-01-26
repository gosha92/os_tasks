#include <stdio.h>
#include <stdlib.h>
#include "vector"
#include "time.h"
#include "pthread.h"
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

using std::vector;

void* testing(void*)
{
	vector<char*> blocks;
	for (size_t k = 0; k < 100000; ++k) {
		double v = drand48();
		if (v < 0.333 || !blocks.size()) { // Выделение памяти
			int l = 256 * (rand() % 4 + 1);
			blocks.push_back(new char[l]);
			continue;
		}
		if (v < 0.666) { // Освобождение памяти
			size_t i = rand() % blocks.size();
			delete[] blocks[i];
			blocks[i] = blocks.back();
			blocks.pop_back();
			continue;
		}
		size_t i = rand() % blocks.size(); // Запись
		blocks[i][100] = 'X';
		continue;
	}
}

int main(void)
{
	srand((int)time(0));
	
	// THREADS //
    double start = (double)clock()/CLOCKS_PER_SEC; // START

    pthread_t threads[20];
    for (size_t i = 0; i < 20; ++i)
	{
        pthread_create(&threads[i], NULL, testing, NULL);
	}
    for (size_t i = 0; i < 20; ++i)
	{
        pthread_join(threads[i], NULL);
    }
    double end = (double)clock()/CLOCKS_PER_SEC;
    printf("Threads time: %0.4f\n", end - start); // END
	// THREADS //

	// PROCESSES //
    start = (double)clock()/CLOCKS_PER_SEC; // START
    pid_t pids[20];
    for(size_t i = 0; i < 20; ++i) {
        pids[i] = fork();
        if (pids[i] == 0) {
            testing(NULL);
            exit(0);
        }
    }
    wait(NULL);
    end = (double)clock()/CLOCKS_PER_SEC;
    printf("Processes time: %0.4f\n", end - start); // END
	// PROCESSES //
}




















