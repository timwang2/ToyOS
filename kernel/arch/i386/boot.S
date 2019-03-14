# Declare constants for the multiboot header.
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set FLAGS,    ALIGN | MEMINFO  # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

# Declare a header as in the Multiboot Standard.
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# Reserve a stack for the initial thread.
.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

#Set up initial upper half paging
.section .bss, "aw", @nobits
.align 4096
boot_PGD:
	.skip 4096
boot_PGT1:
	.skip 4096



# The kernel entry point.
.section .text
.global _start
.type _start, @function
_start:
	# Set up first Page Table
	movl $(boot_PGT1 - 0xC0000000), %edi
	# First entry to set
	movl $0, %esi
	# Couner to maintain the current address to write
	movl $0, %ebx

	
PGT1_loop:
	cmpl $1024, %esi
	je setPGD
	
	orl $0x003, %ebx
	movl %ebx, (%edi)

	addl $4, %edi
	addl $4096, %ebx
	incl %esi
	jmp PGT1_loop

setPGD:
	#set the correct entries of the PGD
	movl $(boot_PGT1 - 0xC0000000 + 0x003), boot_PGD - 0xC0000000
	movl $(boot_PGT1 - 0xC0000000 + 0x003), boot_PGD - 0xC0000000 + 768 * 4

	movl $(boot_PGD - 0xC0000000), %ecx
	movl %ecx, %cr3
	
	#turning on paging
	movl %cr0, %ecx
	orl $0x80010000, %ecx
	movl %ecx, %cr0

	movl $unmapLower, %ecx
	jmp *%ecx

unmapLower:

	movl $0, boot_PGD
	
	movl %cr3, %ecx
	movl %ecx, %cr3

	# set Stack to proper address after paging
	movl $stack_top, %esp

	call _init

	# Transfer control to the main kernel.
	call kernel_main

	# Hang if kernel_main unexpectedly returns.
	cli
1:	hlt
	jmp 1b
.size _start, . - _start
