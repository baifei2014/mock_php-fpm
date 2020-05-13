#include <sys/types.h>
#include <sys/stat.h> /* for chmod(2) */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "worker_pool.h"
#include "fpm.h"

void *get_addr_info(struct sockaddr *sa) /* {{{ */
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int get_listening_socket(struct worker_pool_s *wp, struct sockaddr *sa, int socklen)
{
	int flags = 1;
	int sock;

	sock = socket(sa->sa_family, SOCK_STREAM, 0);

	if (0 > sock) {
		return -1;
	}

	if (0 > setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &flags, sizeof(flags))) {
		printf("failed to change socket attribute");
	}

	if (0 > bind(sock, sa, socklen)) {
		printf("unable to bind listening socket for address");
		close(sock);
		return -1;
	}

	if (0 > listen(sock, wp->config->listen_backlog)) {
		return -1;
	}

	return sock;
}

int new_listening_socket(struct worker_pool_s *wp, const char *addr, const char *port)
{
	struct addrinfo hints, *servinfo, *p;
	char tmpbuf[INET6_ADDRSTRLEN];
	int status;
	int sock = -1;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((status = getaddrinfo(addr, port, &hints, &servinfo)) != 0) {
		return -1;
	}

	for (p = servinfo; p != NULL; p = p->ai_next) {
		inet_ntop(p->ai_family, get_addr_info(p->ai_addr), tmpbuf, INET6_ADDRSTRLEN);
		if (sock < 0) {
			if ((sock = get_listening_socket(wp, p->ai_addr, p->ai_addrlen)) != -1) {
				printf("Found address for %s, socket opened on %s\n", addr, tmpbuf);
			}
		} else {
			printf("Found multiple addresses for %s, %s ignored", addr, tmpbuf);
		}
	}

	freeaddrinfo(servinfo);

	return sock;
}

// int create_socket_by_addr(const char *listening_address)
int create_socket_by_addr(struct worker_pool_s *wp)
{
	char *dup_address = strdup(wp->config->listen_address);
	char *port_str = strrchr(dup_address, ':');
	char *addr = NULL;
	int addr_len;
	int port = 0;
	int sock = -1;

	if (port_str) { /* this is host:port pair */
		*port_str++ = '\0';
		port = atoi(port_str);
		addr = dup_address;

		/* strip brackets from address for getaddrinfo */
		addr_len = strlen(addr);
		if (addr[0] == '[' && addr[addr_len - 1] == ']') {
			addr[addr_len - 1] = '\0';
			addr++;
		}

	} else if (strlen(dup_address) == strspn(dup_address, "0123456789")) { /* this is port */
		port = atoi(dup_address);
		port_str = dup_address;
	}

	if (port == 0) {
		return -1;
	}

	sock = new_listening_socket(wp, addr, port_str);

	free(dup_address);
	return sock;
}

int socket_init_main()
{
	struct worker_pool_s *wp;
	for (wp = worker_all_pools; wp; wp = wp->next)
	{
		wp->listening_socket = create_socket_by_addr(wp);
	}

	return 0;
}
