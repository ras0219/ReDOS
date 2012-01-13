#include "gdt.h"

using namespace GDT;

#pragma pack(push,1)
#pragma pack(pop)

struct GDT_Expanded
{
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_middle;
    uint8_t  access;
    uint8_t  granularity;
    uint8_t  base_high;
};

GDT_Entry gdt[3] = {
    0,
};

void GDT_Table::init()
{
    loc.limit = sizeof(GDT_Entry) * NUM_ENTRIES - 1;
    loc.base = this;
    set_gate(0, 0);
    set_gate(1, 0x00CF9A000000FFFF);
    set_gate(2, 0x00CF92000000FFFF);
}

void GDT_Table::set_gate(size_t eno, uint64_t entry)
{
    *(uint64_t*)(&entries[eno]) = entry;
}

void GDT_Table::set_gate(size_t eno, uint32_t base, uint32_t limit,
              uint8_t access, uint8_t gran)
{
  /* Setup the descriptor base address */
  gdt[eno].base_low    = (base & 0xFFFF);
  gdt[eno].base_middle = (base >> 16) & 0xFF;
  gdt[eno].base_high   = (base >> 24) & 0xFF;

  /* Setup the descriptor limits */
  gdt[eno].limit_low   = (limit & 0xFFFF);
  gdt[eno].granularity = ((limit >> 16) & 0x0F);

  /* Finally, set up the granularity and access flags */
  gdt[eno].granularity |= (gran & 0xF0);
  gdt[eno].access       = access;
}

void GDT_Table::load(uint16_t code_sel)
{
    uint32_t k = (uint32_t)&loc;
    __asm
    {
        lgdt [k]

        // push the address on the stack
        push code_sel
        mov eax, offset flush2
        push eax

        // ret use the previous pushed address
        retf
        flush2:

        mov ax, 0x10
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        mov ss, ax
    }
}
