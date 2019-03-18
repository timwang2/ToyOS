#include <kernel/setup.h>
#include <string.h>

//GDT Entry Low Word Offsets
#define GDTENTRY_LOWORD_LIMIT_0_15_OFFSET                      (0)
#define GDTENTRY_LOWORD_BASE_0_15_OFFSET                       (16)

//GDT Entry High Word offsets
#define GDTENTRY_HIWORD_BASE_16_23_OFFSET                     (0)
#define GDTENTRY_HIWORD_LIMIT_16_19_OFFSET                    (16)
#define GDTENTRY_HIWORD_BASE_24_31_OFFSET                     (24)

#define SEGMENT_ACCESSED_MASK                         (1 << 8)
#define SEGMENT_READWRITE_MASK                        (1 << 9)
#define SEGMENT_DIRECTION_MASK                        (1 << 10)
#define SEGMENT_EXECUTABLE_MASK                       (1 << 11)
#define SEGMENT_SYSTEM_MASK                           (1 << 12)
#define SEGMENT_PRESENT_MASK                          (1 << 15)
#define SEGMENT_SIZE_MASK                             (1 << 22)
#define SEGMENT_GRANULARITY_MASK                      (1 << 23)

#define SEGMENT_USER_PRIO_MASK                                 (3 << 13)

#define FLAT_SEG_LIMIT                                          (0xFFFFF)
#define FLAT_SEG_BASE                                           0

#define GDT_NULL_INDEX                                          0
#define GDT_KERNEL_CODE_INDEX                                   1
#define GDT_KERNEL_DATA_INDEX                                   2

#define ALL_SEGMENT_FLAGS (SEGMENT_PRESENT_MASK | SEGMENT_SIZE_MASK)
#define DATA_SEGMENT_FLAGS (SEGMENT_SYSTEM_MASK | SEGMENT_READWRITE_MASK | SEGMENT_GRANULARITY_MASK)
#define CODE_SEGMENT_FLAGS (DATA_SEGMENT_FLAGS | SEGMENT_EXECUTABLE_MASK)

typedef struct __attribute__((packed)) GDTEntry {
    uint32_t low;
    uint32_t high;
} GDTEntry_t;

typedef struct Segment{
    uint32_t base;
    uint32_t limit;
    uint32_t flags;
} Segment_t;

typedef struct __attribute__((packed)) GDTDesc {
    uint16_t size;
    void * base;
} GDTDesc_t;

/*
 * 0x00 - NULL ENTRY
 * 0x08 - KERNEL CODE
 * 0x10 - KERNEL DATA
 * 0x18 - TSS
 */
GDTEntry_t GDT[6];

/*
For now do not expose these interfaces, If the kernel actually has to do something with these table
*/
void initGdt();
Segment_t initFlatSegment(uint32_t flags);
void initGdtEntry(uint32_t gdtIndex, Segment_t * const targetSegment);
void setGdtRegister();

void segments_initialize( ) {
    initGdt();
    setGdtRegister();
}

void initGdt( ) {
    memset(GDT, 0, sizeof(GDT));
    Segment_t KernelCodeSegment = initFlatSegment(ALL_SEGMENT_FLAGS | CODE_SEGMENT_FLAGS);
    Segment_t KernelDataSegment = initFlatSegment(ALL_SEGMENT_FLAGS | DATA_SEGMENT_FLAGS);

    initGdtEntry(GDT_KERNEL_CODE_INDEX, &KernelCodeSegment);
    initGdtEntry(GDT_KERNEL_DATA_INDEX, &KernelDataSegment);
    return;
}

Segment_t initFlatSegment( uint32_t flags ) {
    Segment_t flatSeg;
    flatSeg.base = FLAT_SEG_BASE;
    flatSeg.limit = FLAT_SEG_LIMIT;
    flatSeg.flags = flags;
    return flatSeg;
}

void initGdtEntry(uint32_t gdtIndex, Segment_t * const targetSegment) {
    if( gdtIndex >= sizeof(GDT) || targetSegment == NULL) {
        return;
    }
    
    GDT[gdtIndex].low |= (targetSegment->base & 0xFFFF) << GDTENTRY_LOWORD_BASE_0_15_OFFSET;
    GDT[gdtIndex].low |= (targetSegment->limit & 0xFFFF) << GDTENTRY_LOWORD_LIMIT_0_15_OFFSET;

    GDT[gdtIndex].high = targetSegment->flags;
    GDT[gdtIndex].high |= ((targetSegment->base >> 16) & 0xFF) << GDTENTRY_HIWORD_BASE_16_23_OFFSET;
    GDT[gdtIndex].high |= ((targetSegment->base >> 24) & 0xFF) << GDTENTRY_HIWORD_BASE_24_31_OFFSET;
    GDT[gdtIndex].high |= ((targetSegment->limit >> 16) & 0xF) << GDTENTRY_HIWORD_LIMIT_16_19_OFFSET;
}

void setGdtRegister() {
    GDTDesc_t targetGdtDesc;
    targetGdtDesc.size = sizeof(GDT);
    targetGdtDesc.base = GDT;
    
    //Refresh the segments using some assembly
    __asm__ volatile ("LGDT %0\n\t"
        "LJMP $0x08, $reload_CS%=\n\t"
        "reload_CS%=:\n\t"
        "MOV $0x10, %%AX \n\t"
        "MOV %%AX, %%DS\n\t"
        "MOV %%AX, %%ES\n\t"
        "MOV %%AX, %%FS\n\t"
        "MOV %%AX, %%GS\n\t"
        "MOV %%AX, %%SS"
        :
        :"m" (targetGdtDesc)
        :"eax"
    );
}
