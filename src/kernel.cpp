#include "kernel.h"
#include "stdio.h"
#include "gdt.h"
#include "idt.h"
#include "irq.h"
#include "vmm.h"

Memory::PhysFramePool krnl_mem;
Memory::PhysFramePool proc_mem;
extern uint32_t __end_of_kernel;

void kernel()
{
    cnsl.init();
    cnsl.set_attribs(VGA::WHITE, VGA::BLACK);
    cnsl.clearscreen();

    cnsl << "Loading GDT, IDT, PICs...            [";
    cnsl.push_attribs(VGA::RED, VGA::BLACK);
    cnsl << "BUSY";
    cnsl.pop_attribs();
    cnsl << ']';

    load_default_gdt();
    KRNL::idt.init();
    KRNL::idt.load();
    IRQ::remap_pics();

    cnsl.push_attribs(VGA::BLUE, VGA::BLACK);
    cnsl << "\b\b\b\b\b OK ";
    cnsl.pop_attribs();
    cnsl << "]\r\n";

    cnsl << "Initializing Keyboard Driver...      [";
    cnsl.push_attribs(VGA::RED, VGA::BLACK);
    cnsl << "BUSY";
    cnsl.pop_attribs();
    cnsl << ']';

    IDT::hook_global(33, Devices::Keyboard::keyboard_isr);
    IRQ::unmask(1);

    cnsl.push_attribs(VGA::BLUE, VGA::BLACK);
    cnsl << "\b\b\b\b\b OK ";
    cnsl.pop_attribs();
    cnsl << "]\r\n";

    __asm sti

    cnsl << "Initializing Virtual Memory...      [";
    cnsl.push_attribs(VGA::RED, VGA::BLACK);
    cnsl << "BUSY";
    cnsl.pop_attribs();
    cnsl << ']';

    // Allocate range of 78 pages (480 KB)
    krnl_mem.init_pool((Memory::PhysFrame*)0x00008000, (Memory::PhysFrame*)0x00080000);
    proc_mem.init_pool((Memory::PhysFrame*)(((uint32_t)&__end_of_kernel + 0x1FFFC) & 0xFFFF000), (Memory::PhysFrame*)0x00200000);

    Memory::PDT * kernelpdt = Memory::init_physical_pdt(&krnl_mem);

    cnsl.push_attribs(VGA::BLUE, VGA::BLACK);
    cnsl << "\b\b\b\b\b OK ";
    cnsl.pop_attribs();
    cnsl << "]\r\n";

    __asm
    {
        mov eax, kernelpdt
        mov cr3, eax
 
        mov eax, cr0
        or eax, 0x80000000
        mov cr0, eax
    }

    for(;;)
    {
        char key = Devices::Keyboard::getc();
        cnsl << key;
    }
}

void __declspec(noreturn) RSoD(const char * info, uint32_t status, REGS * _r)
{
    __asm cli;
    cnsl.set_attribs(VGA::LIGHT_GREY, VGA::RED);
    cnsl.clearscreen();

    cnsl.set_attribs(VGA::RED, VGA::LIGHT_GREY);
    cnsl.clearline(0);
    cnsl.clearline(25);
    cnsl._set_cursor_pos(31,0);
    cnsl << "ReDOS System Error";

    cnsl.set_attribs(VGA::LIGHT_GREY, VGA::RED);
    cnsl._set_cursor_pos(0,2);
    cnsl << "Operating System has encountered a critical error.";
    cnsl._set_cursor_pos(4,5);
    cnsl << "Status: ";
    cnsl.set_attribs(VGA::WHITE, VGA::RED);
    cnsl << (uint64_t)status;

    if (info != NULL)
    {
        cnsl.set_attribs(VGA::LIGHT_GREY, VGA::RED);
        cnsl._set_cursor_pos(4,9);
        cnsl << "Info: ";
        cnsl.set_attribs(VGA::WHITE, VGA::RED);
        cnsl << info;
    }

    if (_r != NULL)
    {
        cnsl._set_cursor_pos(0,13);
        cnsl << *_r;
    }

    __asm hlt;
}

IDT::IDT_Table KRNL::idt = { 0 };