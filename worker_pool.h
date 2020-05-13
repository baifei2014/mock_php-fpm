#ifndef WORKER_POOL_H
#define WORKER_POOL_H 1

#include "conf.h"

struct worker_pool_s;
struct child_s;

struct worker_pool_s {
	struct worker_pool_s *next;
	struct worker_config *config;
	int listening_socket;
	int running_children;
	struct child_s *children;
};

struct worker_pool_s *worker_pool_alloc();

extern struct worker_pool_s *worker_all_pools;

#endif