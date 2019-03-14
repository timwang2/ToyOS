#ifndef KERNEL_SETUP_H
#define KERNEL_SETUP_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define GDTSELECTOR_NULL                                (0x00)
#define GDTSELECTOR_KERNEL_CODE                         (0x08)
#define GDTSELECTOR_KERNEL_DATA                         (0x10)
#define GDTSELECTOR_TSS                                 (0x18)

typedef struct __attribute__((packed)) GDTEntry {
    uint32_t low;
    uint32_t high;
 } GDTEntry_t;

/*
 * 0x00 - NULL ENTRY
 * 0x08 - KERNEL CODE
 * 0x10 - KERNEL DATA
 * 0x18 - TSS
 */
GDTEntry_t GDT[6];

void initFlatGDT( );
void initGdtEntry(GDTEntry_t *target, uint32_t base, uint32_t limit, uint32_t flags);




#endif //SETUP_H
