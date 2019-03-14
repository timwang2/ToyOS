#ifndef KERNEL_SETUP_H
#define KERNEL_SETUP_H

//GDT Entry Low Word Offsets
#define GDTENTRY_LOW_LIMIT_0_15_OFFSET                  (0)
#define GDTENTRY_LOW_BASE_0_15_OFFSET                   (16)

//GDT Entry High 4 byte word offsets
#define GDTENTRY_HIGH_BASE_16_23_OFFSET                 (0)
#define GDTENTRY_HIGH_LIMIT_16_19_OFFSET                (16)
#define GDTENTRY_HIGH_BASE_24_31_OFFSET                 (24)

#define GDTENTRY_HIGH_ACCESSED_MASK                     (1 << 8)
#define GDTENTRY_HIGH_READWRITE_MASK                    (1 << 9)
#define GDTENTRY_HIGH_DIRECTION_MASK                    (1 << 10)
#define GDTENTRY_HIGH_EXECUTABLE_MASK                   (1 << 11)
#define GDTENTRY_HIGH_SYSTEM_MASK                       (1 << 12)
#define GDTENTRY_HIGH_PRESENT_MASK                      (1 << 15)
#define GDTENTRY_HIGH_SIZE_MASK                         (1 << 22)
#define GDTENTRY_HIGH_GRANULARITY_MASK                  (1 << 23)

#define GDTENTRY_USER_PRIO_MASK                         (3 << 13)

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
