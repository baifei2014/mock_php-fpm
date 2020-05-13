#ifndef CHILDREN_H
#define CHILDREN_H 1

#include <unistd.h>

#include "worker_pool.h"
#include "worker_pool.h"

void child_init(struct worker_pool_s *wp);
int children_make(struct worker_pool_s *wp);

struct child_s;

struct child_s {
	struct child_s *prev, *next;
	struct worker_pool_s *wp;
	pid_t pid;
};

#endif
