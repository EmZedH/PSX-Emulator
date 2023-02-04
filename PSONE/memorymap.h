#pragma once
#include "definedclasses.h"
class Map
{
public:
    bool contains(const Range *range, u32 addr)
    {
        return ((addr >= range->start) && (addr < (range->start + range->length)));
    }

    u32 maskRegion(u32 addr)
    {
        return (addr & REGION_MASK[addr >> 29]);
    }
};
    const Range BIOS_SIZE = Range(0x1fc00000, 512 * 1024);
    const Range SYS_CONTROL = Range(0x1f801000, 36); // SYS_CONTROL AND MEM_CONTROL
    const Range RAM_REG_SIZE = Range(0x1f801060, 4);
    const Range CACHE_CONTROL = Range(0xfffe0130, 4);
    const Range RAM_SIZE = Range(0x00000000, 2 * 1024 * 1024);
    const Range SPU = Range(0x1f801c00, 640);
    const Range EXPANSION_2 = Range(0x1f802000, 66);
    const Range EXPANSION_1 = Range(0x1f000000, 8192 * 1024);
    const Range IRQ_CONTROL = Range(0x1f801070, 8);
    const Range TIMERS = Range(0x1f801100, 0x30);
    const Range DMA_SIZE = Range(0x1f801080,0x80);
    const Range GPU_SIZE = Range(0x1f801810,0x5);