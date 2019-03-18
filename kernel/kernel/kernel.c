#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/setup.h>

void kernel_main(void) {

    segments_initialize();
	terminal_initialize();
	printf("Hello, kernel World!\n");
}
