#ifndef CONF_H
#define CONF_H 1

struct worker_config {
	char *listen_address;
	int listen_backlog;
	int pm;
	int pm_max_children;
	int pm_start_servers;
};

void worker_pool_init_main();

enum {
	PM_STYLE_STATIC = 1,
	PM_STYLE_DYNAMIC = 2,
	PM_STYLE_ONDEMAND = 3
};

#endif