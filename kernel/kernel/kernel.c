#include <stdio.h>

#include <kernel/tty.h>

void kernel_main(void) {

    gdt_initialize();
    ivt_initialize();
    
	terminal_initialize();
	printf("Hello, kernel World!\n");
}
