#include <kernel/setup.h>
#include <string.h>

//GDT Entry Low Word Offsets
#define GDTENTRY_LW_LIMIT_0_15_OFFSET                  (0)
#define GDTENTRY_LW_BASE_0_15_OFFSET                   (16)

//GDT Entry High Word offsets
#define GDTENTRY_HW_BASE_16_23_OFFSET                 (0)
#define GDTENTRY_HW_LIMIT_16_19_OFFSET                (16)
#define GDTENTRY_HW_BASE_24_31_OFFSET                 (24)

#define GDTENTRY_HW_ACCESSED_MASK                     (1 << 8)
#define GDTENTRY_HW_READWRITE_MASK                    (1 << 9)
#define GDTENTRY_HW_DIRECTION_MASK                    (1 << 10)
#define GDTENTRY_HW_EXECUTABLE_MASK                   (1 << 11)
#define GDTENTRY_HW_SYSTEM_MASK                       (1 << 12)
#define GDTENTRY_HW_PRESENT_MASK                      (1 << 15)
#define GDTENTRY_HW_SIZE_MASK                         (1 << 22)
#define GDTENTRY_HW_GRANULARITY_MASK                  (1 << 23)

#define GDTENTRY_USER_PRIO_MASK                       (3 << 13)

#define GDTENTRY_LIMIT_MAX                            (0xFFFFF)


void initFlatGDT( ) {
    memset(GDT, 0, sizeof(GDT));

    uint32_t flagsKernData = GDTENTRY_HW_PRESENT_MASK |
                             GDTENTRY_HW_SYSTEM_MASK  |
                             GDTENTRY_HW_READWRITE_MASK |
                             GDTENTRY_HW_GRANULARITY_MASK | 
                             GDTENTRY_HW_SIZE_MASK;
            
    uint32_t flagsKernCode = flagsKernData | GDTENTRY_HW_EXECUTABLE_MASK;
    //Set up kernel data code descriptor
    initGdtEntry(&GDT[1], 0, GDTENTRY_LIMIT_MAX, flagsKernCode);
    //Set up kernel data segment descriptor
    initGdtEntry(&GDT[2], 0, GDTENTRY_LIMIT_MAX, flagsKernData);
    return;
}

void initGdtEntry(GDTEntry_t *targetGdtEntry, uint32_t base, uint32_t limit, uint32_t flags) {
    if(targetGdtEntry == NULL) {
        return;
    }
    
    targetGdtEntry->low |= (base & 0xFFFF) << GDTENTRY_LW_BASE_0_15_OFFSET;
    targetGdtEntry->low |= (limit & 0xFFFF) << GDTENTRY_LW_LIMIT_0_15_OFFSET;

    targetGdtEntry->high = flags;
    targetGdtEntry->high |= ((base >> 16) & 0xFF) << GDTENTRY_HW_BASE_16_23_OFFSET;
    targetGdtEntry->high |= ((base >> 24) & 0xFF) << GDTENTRY_HW_BASE_24_31_OFFSET;
    targetGdtEntry->high |= ((limit >> 16) & 0xF) << GDTENTRY_HW_LIMIT_16_19_OFFSET;
}
