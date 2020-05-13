#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "worker_pool.h"
#include "conf.h"

void worker_pool_init_main()
{
	struct worker_pool_s *wp;

	wp = worker_pool_alloc();

	wp->config = malloc(sizeof(struct worker_config));
	if (!wp->config) {
		free(wp->config);
		free(wp);
		printf("%s\n", "内存分配失败");
	}

	memset(wp->config, 0, sizeof(struct worker_config));
	wp->config->listen_address = "0.0.0.0:9000";
	wp->config->listen_backlog = 5;
	wp->config->pm = PM_STYLE_DYNAMIC;
	wp->config->pm_max_children = 20;
	wp->config->pm_start_servers = 3;

	if (!worker_all_pools) {
		worker_all_pools = wp;
	} else {
		struct worker_pool_s *tmp = worker_all_pools;
		while (tmp) {
			if (!tmp->next) {
				tmp->next = wp;
				break;
			}
			tmp = tmp->next;
		}
	}
}