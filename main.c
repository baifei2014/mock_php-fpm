#include <stdio.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>

#include "fpm.h"


static bool stop = false;
static void handle_term(int sig)
{
	stop = true;
}

int main()
{
	signal(SIGTERM, handle_term);

	init();

	run();


	while(!stop) {
		printf("%d\n", getpid());
		sleep(2);
	}

	return 0;
}