#include "idt.h"
#include "machine.h"
#include "stdio.h"
#include "kernel.h"

using namespace IDT;

/*
From: http://www.logix.cz/michal/doc/i386/chp09-00.htm

0            Divide error
1            Debug exceptions
2            Nonmaskable interrupt
3            Breakpoint (one-byte INT 3 instruction)
4            Overflow (INTO instruction)
5            Bounds check (BOUND instruction)
6            Invalid opcode
7            Coprocessor not available
8            Double fault
9            (reserved)
10           Invalid TSS
11           Segment not present
12           Stack exception
13           General protection
14           Page fault
15           (reserved)
16           Coprecessor error
17-31        (reserved)
32-255       Available for external interrupts via INTR pin
*/

static ISR global_isrs[IDT_Table::NUM_ENTRIES] = { 0 };

ISR IDT::hook_global(uint32_t gateno, ISR isr)
{
    assert(gateno < IDT_Table::NUM_ENTRIES);
    ISR old_isr = global_isrs[gateno];
    global_isrs[gateno] = isr;
    return old_isr;
}

void gnrl_exc_dispatch(REGS * _r)
{
    assert(_r->int_no < IDT_Table::NUM_ENTRIES);
    if (_r->int_no == 39)
    {
        // spurious interrupt, ignore.
        return;
    }

    if (global_isrs[_r->int_no] == NULL)
    {
        RSoD("Unregistered Interrupt", *(uint32_t*)(_r->eip), _r);
    }

    if (global_isrs[_r->int_no](_r))
    {
        RSoD("Unhandled Interrupt", *(uint32_t*)(_r->eip), _r);
    }
    if (_r->int_no > 31 && _r->int_no < 48)
    {
        // master PIC must be reset
        outb(0x20,0x20);
    }
    if (_r->int_no > 39 && _r->int_no < 48)
    {
        // slave PIC must be reset
        outb(0xA0,0x20);
    }
}

void __declspec(naked) _isr_common()
{
    __asm
    {
        pushad // GRAR pusha != pushad
        push ds
        push es
        push fs
        push gs
        mov ax, 0x10   // Load the kernel data segment descriptor!
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        mov eax, esp   // Push us the stack
        push eax
        mov eax, gnrl_exc_dispatch
        call eax	// A special call, preserves the 'eip' register
        pop eax
        pop gs
        pop fs
        pop es
        pop ds
        popad
        add esp, 8	// Cleans up the pushed error code and pushed ISR number
        iretd	// pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP1
    }
}

void IDT_Table::set_gate(uint16_t gateno,
                         void (*isr)(),
                         uint16_t selector,
                         uint8_t flags)
{
    entries[gateno].base_hi = (uint32_t)isr >> 16;
    entries[gateno].base_lo = (uint32_t)isr & 0xFFFF;
    entries[gateno].flags = flags;
    entries[gateno].sel = selector;
}

void _isr0();
void _isr1();
void _isr2();
void _isr3();
void _isr4();
void _isr5();
void _isr6();
void _isr7();
void _isr8();
void _isr9();
void _isr10();
void _isr11();
void _isr12();
void _isr13();
void _isr14();
void _isr16();

void _isr32();
void _isr33();
void _isr34();
void _isr35();
void _isr36();
void _isr37();
void _isr38();
void _isr39();
void _isr40();
void _isr41();
void _isr42();
void _isr43();
void _isr44();
void _isr45();
void _isr46();
void _isr47();

void IDT_Table::init()
{
    set_gate(0, _isr0);
    set_gate(1, _isr1);
    set_gate(2, _isr2);
    set_gate(3, _isr3);
    set_gate(4, _isr4);
    set_gate(5, _isr5);
    set_gate(6, _isr6);
    set_gate(7, _isr7);
    set_gate(8, _isr8);
    set_gate(9, _isr9);
    set_gate(10, _isr10);
    set_gate(11, _isr11);
    set_gate(12, _isr12);
    set_gate(13, _isr13);
    set_gate(14, _isr14);
    set_gate(16, _isr16);

    set_gate(32, _isr32);
    set_gate(33, _isr33);
    set_gate(34, _isr34);
    set_gate(35, _isr35);
    set_gate(36, _isr36);
    set_gate(37, _isr37);
    set_gate(38, _isr38);
    set_gate(39, _isr39);
    set_gate(40, _isr40);
    set_gate(41, _isr41);
    set_gate(42, _isr42);
    set_gate(43, _isr43);
    set_gate(44, _isr44);
    set_gate(45, _isr45);
    set_gate(46, _isr46);
    set_gate(47, _isr47);

    loc.limit = IDT_Table::NUM_ENTRIES * sizeof(IDT_Entry) - 1;
    loc.base = this;
}

void __declspec(naked) _isr0()
{
    __asm
    {
        push 0
        push 0
        jmp _isr_common
    }
}
void __declspec(naked) _isr1()
{
    __asm
    {
        push 0
        push 1
        jmp _isr_common
    }
}
void __declspec(naked) _isr2()
{
    __asm
    {
        push 0
        push 2
        jmp _isr_common
    }
}
void __declspec(naked) _isr3()
{
    __asm
    {
        push 0
        push 3
        jmp _isr_common
    }
}
void __declspec(naked) _isr4()
{
    __asm
    {
        push 0
        push 4
        jmp _isr_common
    }
}
void __declspec(naked) _isr5()
{
    __asm
    {
        push 0
        push 5
        jmp _isr_common
    }
}
void __declspec(naked) _isr6()
{
    __asm
    {
        push 0
        push 6
        jmp _isr_common
    }
}
void __declspec(naked) _isr7()
{
    __asm
    {
        push 0
        push 7
        jmp _isr_common
    }
}
void __declspec(naked) _isr8()
{
    __asm
    {
        push 8
        jmp _isr_common
    }
}
void __declspec(naked) _isr9()
{
    __asm
    {
        push 0
        push 9
        jmp _isr_common
    }
}
void __declspec(naked) _isr10()
{
    __asm
    {
        push 10
        jmp _isr_common
    }
}
void __declspec(naked) _isr11()
{
    __asm
    {
        push 11
        jmp _isr_common
    }
}
void __declspec(naked) _isr12()
{
    __asm
    {
        push 12
        jmp _isr_common
    }
}
void __declspec(naked) _isr13()
{
    __asm
    {
        push 13
        jmp _isr_common
    }
}
void __declspec(naked) _isr14()
{
    __asm
    {
        push 14
        jmp _isr_common
    }
}
void __declspec(naked) _isr16()
{
    __asm
    {
        push 0
        push 16
        jmp _isr_common
    }
}

void __declspec(naked) _isr32()
{
    __asm
    {
        push 0
        push 32
        jmp _isr_common
    }
}
void __declspec(naked) _isr33()
{
    __asm
    {
        push 0
        push 33
        jmp _isr_common
    }
}
void __declspec(naked) _isr34()
{
    __asm
    {
        push 0
        push 34
        jmp _isr_common
    }
}
void __declspec(naked) _isr35()
{
    __asm
    {
        push 0
        push 35
        jmp _isr_common
    }
}
void __declspec(naked) _isr36()
{
    __asm
    {
        push 0
        push 36
        jmp _isr_common
    }
}
void __declspec(naked) _isr37()
{
    __asm
    {
        push 0
        push 37
        jmp _isr_common
    }
}
void __declspec(naked) _isr38()
{
    __asm
    {
        push 38
        jmp _isr_common
    }
}
void __declspec(naked) _isr39()
{
    __asm
    {
        push 0
        push 39
        jmp _isr_common
    }
}
void __declspec(naked) _isr40()
{
    __asm
    {
        push 40
        jmp _isr_common
    }
}
void __declspec(naked) _isr41()
{
    __asm
    {
        push 41
        jmp _isr_common
    }
}
void __declspec(naked) _isr42()
{
    __asm
    {
        push 42
        jmp _isr_common
    }
}
void __declspec(naked) _isr43()
{
    __asm
    {
        push 43
        jmp _isr_common
    }
}
void __declspec(naked) _isr44()
{
    __asm
    {
        push 44
        jmp _isr_common
    }
}
void __declspec(naked) _isr45()
{
    __asm
    {
        push 0
        push 45
        jmp _isr_common
    }
}
void __declspec(naked) _isr46()
{
    __asm
    {
        push 0
        push 46
        jmp _isr_common
    }
}
void __declspec(naked) _isr47()
{
    __asm
    {
        push 0
        push 47
        jmp _isr_common
    }
}
