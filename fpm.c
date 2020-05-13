#include <sys/time.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdio.h>

#include "socket.h"
#include "worker_pool.h"
#include "children.h"
#include "fpm.h"
#include "event.h"

struct globals_s globals = {
	.parent_pid = 0,
	.listening_socket = 0,
	.running_children = 0,
};

int run(void)
{
	struct worker_pool_s *wp;

	for (wp = worker_all_pools; wp; wp = wp->next)
	{
		int is_parent;
		is_parent = children_make(wp);
		if (!is_parent) {
			goto run_child;
		}
	}

	event_loop();

run_child:
	return 1;
}

int init()
{
	worker_pool_init_main();

	socket_init_main();

	return 0;
}