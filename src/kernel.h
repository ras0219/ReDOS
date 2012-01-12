#ifndef _KERNEL_H_
#define _KERNEL_H_

#include "stdinc.h"
#include "machine.h"
#include "idt.h"

void kernel();

void __declspec(noreturn) RSoD(const char * info,
                               uint32_t status,
                               REGS * _r = NULL);

namespace KRNL
{
    #define KRNL_STACK_SIZE 0x2000
    //const size_t STACK_SIZE = 0x2000; Didn't properly optimize, for some reason
    extern IDT::IDT_Table idt;
}

#endif