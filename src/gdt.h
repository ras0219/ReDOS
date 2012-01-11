#ifndef _GDT_H_
#define _GDT_H_

#include "stdinc.h"

#pragma pack(push,1)
union GDT_Entry
{
    uint64_t entry;
    struct
    {
        uint16_t limit_low;
        uint16_t base_low;
        uint8_t  base_middle;
        uint8_t  access;
        uint8_t  granularity;
        uint8_t  base_high;
    };
};
#pragma pack(pop)

extern GDT_Entry gdt[3];

void load_default_gdt();

#endif