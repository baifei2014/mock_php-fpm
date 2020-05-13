int run(void);
int init();

struct globals_s {
	pid_t parent_pid;
	int listening_socket; /* for this child */
	int running_children;
};

extern struct globals_s globals;