#ifndef KERNEL_SETUP_H
#define KERNEL_SETUP_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define GDTSELECTOR_NULL                                (0x00)
#define GDTSELECTOR_KERNEL_CODE                         (0x08)
#define GDTSELECTOR_KERNEL_DATA                         (0x10)
#define GDTSELECTOR_TSS                                 (0x18)

void segments_initialize();

#endif //SETUP_H
