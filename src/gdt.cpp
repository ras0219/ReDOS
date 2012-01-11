#include "gdt.h"

#pragma pack(push,1)
struct GDT_Table_Loc
{
    uint16_t limit;
    GDT_Entry * base;
};
#pragma pack(pop)

GDT_Entry gdt[3] = {
    0,
    0x00CF9A000000FFFF,
    0x00CF92000000FFFF,
};
static GDT_Table_Loc loc = {
    sizeof(GDT_Entry)*3 - 1,
    gdt
};

void set_gdt_gate(int num, 
                   unsigned long base, unsigned long limit, 
                   unsigned char access, unsigned char gran) {

  /* Setup the descriptor base address */
  gdt[num].base_low    = (base & 0xFFFF);
  gdt[num].base_middle = (base >> 16) & 0xFF;
  gdt[num].base_high   = (base >> 24) & 0xFF;

  /* Setup the descriptor limits */
  gdt[num].limit_low   = (limit & 0xFFFF);
  gdt[num].granularity = ((limit >> 16) & 0x0F);

  /* Finally, set up the granularity and access flags */
  gdt[num].granularity |= (gran & 0xF0);
  gdt[num].access       = access;
}

void load_default_gdt()
{
    __asm
    {
        lgdt [loc]

        // push the address on the stack
        push 0x08
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