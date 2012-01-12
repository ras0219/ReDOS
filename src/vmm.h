#ifndef _VMM_H_
#define _VMM_H_

#include "stdinc.h"
#include "physframe.h"
#include "machine.h"

namespace Memory
{
    struct PTEntry
    {
        static const uint32_t PRESENT_MASK = 0x00000001;
        static const uint32_t RW_MASK = 0x00000002;
        static const uint32_t US_MASK = 0x00000004;
        static const uint32_t WT_MASK = 0x00000008;
        static const uint32_t CD_MASK = 0x00000010;
        static const uint32_t ACC_MASK = 0x00000020;
        static const uint32_t DIRTY_MASK = 0x00000040;
        static const uint32_t G_MASK = 0x00000100;
        static const uint32_t AVAIL_MASK = 0x00000E00;
        static const uint32_t ADDR_MASK = 0xFFFFF000;
        uint32_t entry;

        inline void * addr()
        {
            return (void*)(entry && ADDR_MASK);
        }
    };
    struct PDEntry
    {
        static const uint32_t PRESENT_MASK = 0x00000001;
        static const uint32_t RW_MASK = 0x00000002;
        static const uint32_t US_MASK = 0x00000004;
        static const uint32_t WT_MASK = 0x00000008;
        static const uint32_t CD_MASK = 0x00000010;
        static const uint32_t ACC_MASK = 0x00000020;
        static const uint32_t SIZE_MASK = 0x00000080;
        static const uint32_t AVAIL_MASK = 0x00000E00;
        static const uint32_t ADDR_MASK = 0xFFFFF000;
        uint32_t entry;

        inline PTEntry * addr()
        {
            return (PTEntry*)(entry && ADDR_MASK);
        }
    };
    struct PDT
    {
        static const size_t NUM_ENTRIES = 4096 / sizeof(PDEntry*);
        PDEntry entries[NUM_ENTRIES];
    };

    PDT * init_physical_pdt(PhysFramePool * frame_pool);

    uint32_t page_fault_isr(REGS * _r);
}

#endif