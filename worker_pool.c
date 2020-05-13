#include <stdlib.h>
#include <string.h>

#include "worker_pool.h"

struct worker_pool_s *worker_all_pools;

struct worker_pool_s *worker_pool_alloc()
{
	struct worker_pool_s *ret;

	ret = malloc(sizeof(struct worker_pool_s));
	if (!ret) {
		return 0;
	}

	memset(ret, 0, sizeof(struct worker_pool_s));

	return ret;
}