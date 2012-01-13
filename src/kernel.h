#ifndef _KERNEL_H_
#define _KERNEL_H_

#include "stdinc.h"
#include "machine.h"
#include "idt.h"
#include "gdt.h"
#include "vmm.h"

void __declspec(noreturn) RSoD(const char * info,
                               uint32_t status,
                               REGS * _r = NULL);

namespace KRNL
{
    void kernel();

    #define KRNL_STACK_SIZE 0x2000
    //const size_t STACK_SIZE = 0x2000; Didn't properly optimize, for some reason
    extern IDT::IDT_Table idt;
    extern GDT::GDT_Table gdt;
    extern Memory::PDirectory * kernelpd;
}

#endif