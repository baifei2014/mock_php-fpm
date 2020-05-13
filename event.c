#include <stdio.h>
#include <unistd.h>

void event_loop() {
	while(1) {
		sleep(6);
		printf("%s\n", "你好世界");
	}
}