#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "worker_pool.h"
#include "children.h"
#include "fpm.h"

struct child_s *child_alloc()
{
	struct child_s *ret;
	ret = malloc(sizeof(struct child_s));

	if (!ret) {
		return 0;
	}

	memset(ret, 0, sizeof(*ret));
	return ret;
}

void child_init(struct worker_pool_s *wp)
{
	globals.listening_socket = wp->listening_socket;
}

struct child_s *resources_prepare(struct worker_pool_s *wp)
{
	struct child_s *c;

	c = child_alloc();

	if (!c) {
		return 0;
	}

	c->wp = wp;

	return c;
}

void parent_resources_use(struct child_s *child)
{
	struct worker_pool_s *wp = child->wp;

	++wp->running_children;
	++globals.running_children;

	child->next = wp->children;
	if (child->next) {
		child->next->prev = child;
	}
	child->prev = 0;
	wp->children = child;
}

int children_make(struct worker_pool_s *wp)
{
	struct child_s *child;
	pid_t pid;
	int max;

	max = wp->config->pm_start_servers;

	while (wp->running_children < max) {

		child = resources_prepare(wp);

		pid = fork();

		switch (pid) {
			case 0 :
				printf("%s\n", "子进程");
				child_init(wp);
				return 0;

			default :
				child->pid = pid;
				parent_resources_use(child);
				printf("%d, %s\n", pid, "父进程");
		}
	}

	return 1;
}