#ifndef _VMM_H_
#define _VMM_H_

#include "stdinc.h"
#include "physframe.h"

namespace Memory
{
    struct PDEntry
    {
        static const uint32_t PRESENT_MASK = 0x00000001;
        static const uint32_t RW_MASK = 0x00000002;
        static const uint32_t US_MASK = 0x00000004;
        static const uint32_t ACC_MASK = 0x00000020;
        static const uint32_t DIRTY_MASK = 0x00000040;
        static const uint32_t AVAIL_MASK = 0x00000E00;
        static const uint32_t ADDR_MASK = 0xFFFFF000;
        uint32_t entry;

        inline PTEntry * addr()
        {
            return (PTEntry*)(entry && ADDR_MASK);
        }
    };
    struct PTEntry
    {
        static const uint32_t PRESENT_MASK = 0x00000001;
        static const uint32_t RW_MASK = 0x00000002;
        static const uint32_t US_MASK = 0x00000004;
        static const uint32_t ACC_MASK = 0x00000020;
        static const uint32_t DIRTY_MASK = 0x00000040;
        static const uint32_t AVAIL_MASK = 0x00000E00;
        static const uint32_t ADDR_MASK = 0xFFFFF000;
        uint32_t entry;

        inline void * addr()
        {
            return (void*)(entry && ADDR_MASK);
        }
    };
    struct PDT
    {
        static const size_t NUM_ENTRIES = 1024;
        PDEntry entries[NUM_ENTRIES];
    };

    void init_kernel_pagetable(PhysFramePool * kernel_mem);
}

#endif