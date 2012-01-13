#ifndef _IDT_H_
#define _IDT_H_

#include "stdinc.h"
#include "machine.h"

namespace IDT
{
    #pragma pack(push, 1)
    struct IDT_Entry
    {
        uint16_t base_lo;
        uint16_t sel;
        uint8_t always0;
        uint8_t flags;
        uint16_t base_hi;
    };

    struct IDT_Table;

    struct IDT_Loc
    {
        uint16_t limit;
        IDT_Table * base;
    };

    struct IDT_Table
    {
        static const size_t NUM_ENTRIES = 50;
        IDT_Entry entries[NUM_ENTRIES];
        IDT_Loc loc;

        void init();
        void set_gate(uint16_t gateno,
                      void (*isr)(),
                      uint16_t selector=0x08,
                      uint8_t type_attr=0x8F);
        inline void load()
        {
            __lidt(&this->loc);
        }
    };
    #pragma pack(pop)

    typedef uint32_t (*ISR)(REGS * _r);

    ISR hook_global(uint32_t gateno, ISR isr);
}
#endif