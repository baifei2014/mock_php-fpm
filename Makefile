# Top level makefile, the real shit is at src/Makefile

default: all

.DEFAULT:
	gcc conf.c fpm.c main.c socket.c worker_pool.c children.c event.c -o fpm
install:
	gcc conf.c fpm.c main.c socket.c worker_pool.c children.c event.c -o fpm

.PHONY: install
