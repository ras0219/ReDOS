#ifndef _GDT_H_
#define _GDT_H_

#include "stdinc.h"

namespace GDT
{
    #pragma pack(push,1)
    struct GDT_Entry
    {
        uint16_t limit_low;
        uint16_t base_low;
        uint8_t  base_middle;
        uint8_t  access;
        uint8_t  granularity;
        uint8_t  base_high;
    };

    struct GDT_Table;

    struct GDT_Loc
    {
        uint16_t limit;
        GDT_Table * base;
    };

    struct GDT_Table
    {
        static const size_t NUM_ENTRIES = 3;
        GDT_Entry entries[NUM_ENTRIES];
        GDT_Loc loc;

        void init();
        void set_gate(size_t eno, uint64_t entry);
        void set_gate(size_t eno, uint32_t base, uint32_t limit,
                      uint8_t access, uint8_t gran);
        void load(uint16_t code_sel=0x08);
    };
    #pragma pack(pop)
}

#endif