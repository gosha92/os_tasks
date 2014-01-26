#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <signal.h>

/*
Print "1" 1 second
Print "2" 2 seconds
*/

ucontext_t context_1, context_2;

void print_1(void) // Infinite print 1
{
	while(1) printf("1");
}

void print_2(void) // Infinite print 2
{
	while(1) printf("2");
}

int now_context; // Current context

void toggle_context(int sig) // Toggle between context_1 and context_2
{
	if (now_context == 1)
	{
		now_context = 2;
		alarm(2);
		swapcontext(&context_1, &context_2);
	}
	else
	{
		now_context = 1;
		alarm(1);
		swapcontext(&context_2, &context_1);
	}
}

int main(void)
{
	getcontext(&context_1); // Init context_1
	context_1.uc_stack.ss_sp = malloc(SIGSTKSZ);
	context_1.uc_stack.ss_size = SIGSTKSZ;
	makecontext(&context_1, print_1, 0);
	
	getcontext(&context_2); // Init context_2
	context_2.uc_stack.ss_sp = malloc(SIGSTKSZ);
	context_2.uc_stack.ss_size = SIGSTKSZ;
	makecontext(&context_2, print_2, 0);
	
	now_context = 1;
	signal(SIGALRM, toggle_context);
	alarm(1);
	setcontext(&context_1);
}

