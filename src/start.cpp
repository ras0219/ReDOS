#include "stdinc.h"
#include "kernel.h"

// Disable warning about unary minus on unsigned integers
#pragma warning(disable:4146)

// Disable warning about default constructors and assignments
#pragma warning(disable:4510)
#pragma warning(disable:4512)
#pragma warning(disable:4610)

// Ensure our critical data gets linked in the right order
#pragma comment(linker,"/merge:.text=.two")
#pragma comment(linker,"/merge:.abs=.two")
#pragma comment(linker,"/merge:.data=.two")
#pragma comment(linker,"/merge:.edata=.two")
#pragma comment(linker,"/merge:.rdata=.two")
#pragma comment(linker,"/merge:.bss=.two")

#pragma data_seg(push, ".rsrc$")
// Inject symbol at end of kernel to know size
uint32_t __end_of_kernel = 0;
#pragma data_seg(pop)

// Forward declaration
extern "C" __declspec(noreturn) void __kernel_entry();

// BEGIN MULTIBOOT HEADER
struct MultibootHeader
{
    const uint32_t magic;
    const uint32_t flags;
    const uint32_t checksum;
    const uint32_t header_addr;
    const uint32_t load_addr;
    const uint32_t load_end_addr;
    const uint32_t bss_end_addr;
    const uint32_t entry_addr;
    const uint32_t mode_type;
    const uint32_t width;
    const uint32_t height;
    const uint32_t depth;
};

#pragma data_seg(push, ".abs")
MultibootHeader mbhdr = {
    0x1BADB002, // magic
    1 << 16, // flags
    -(mbhdr.magic + mbhdr.flags), // checksum
    //-(0x1BADB002 + 1 << 16), // checksum
    (uint32_t)&mbhdr, // header_addr
    (uint32_t)&mbhdr, //0x00101000, // load_addr
    (uint32_t)&__end_of_kernel, // load_end_addr
    (uint32_t)&__end_of_kernel, // bss_end_addr
    (uint32_t)&__kernel_entry, // entry_addr
    0, // mode_type
    0, // width
    0, // height
    0 // depth/**/
};
// END MULTIBOOT HEADER
#pragma data_seg(pop)

extern "C" __declspec(naked) __declspec(noreturn) void __kernel_entry()
{
    __asm
    {
        lea esp, [__end_of_kernel + 0x1FFFC]
        and esp, 0xFFFFF000
        xor ecx, ecx
        push ecx
        popf // clear EFLAGS

        // These register values are passed from grub (they're important!)
        push eax
        push ebx
    }

    KRNL::kernel();

    for(;;)
    {
        __halt();
    }
}
