#ifndef _MACHINE_H_
#define _MACHINE_H_

#include "stdinc.h"

struct REGS {
    /* segment descriptors */
    uint32_t gs;
    uint32_t fs;
    uint32_t es;
    uint32_t ds;

    /* General purpose registers, managed by pusha/popa in x86. */
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;

    /* In the low-level handlers (i.e. before we call the 
       exception or interrupt dispatcher) we push number
       of the interrupt and error code onto the stack. */
    uint32_t int_no;
    uint32_t err_code;

    /* These registers are pushed on the stack when 
       the exception occurs. */
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;

    /* These registers are pushed on the stack only when
       the exception occured in user mode.
       As long as you are in kernel mode, the register context
       is 68 bytes long, and not 76! */
    uint32_t useresp;
    uint32_t ss;    
};

#endif