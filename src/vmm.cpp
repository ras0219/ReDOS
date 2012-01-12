#include "vmm.h"
#include "kernel.h"

using namespace Memory;

PhysFramePool * Memory::kernel_mem_pool = NULL;
PhysFramePool * Memory::process_mem_pool = NULL;
static PDirectory *const current_paged_pd = (PDirectory*)0xFFFFF000;

PDirectory * Memory::init_physical_pdt(PhysFramePool * fp)
{
    // kernel page directory
    PDirectory * pd = (PDirectory*) fp -> alloc_frame();

    // identity mapped region page table
    // (0 - 4 MB)
    PTable * pt = (PTable*) fp -> alloc_frame();

    for(size_t eno = 0; eno < 1024; ++eno)
    {
        pd -> entries[eno] = 0;
        pt -> entries[eno] = eno << 12
            | PTable::PRESENT_MASK;
    }
    // Identity mapped address
    pd -> entries[0] = (uint32_t)pt | PDirectory::PRESENT_MASK;
    // Loopback Address
    pd -> entries[1023] = (uint32_t)pd | PDirectory::PRESENT_MASK;

    return pd;
}

static const uint32_t PF_US_MASK = 0x4;
static const uint32_t PF_RW_MASK = 0x2;
static const uint32_t PF_P_MASK = 0x1;

uint32_t Memory::page_fault_isr(REGS * _r)
{
    // Retrieve faulting address
    size_t faddr;
    __asm
    {
        mov eax, cr2
        mov faddr, eax
    }

    if (_r -> err_code & ~PF_RW_MASK)
    {
        // User mode error or protection-based error.
        // Can't handle that.
        return 0;
    }
    // Handling a Supervisor Non-Present error.

    if (kernel_mem_pool == NULL)
    {
        RSoD("No kernel memory pool.", faddr, _r);
    }
    if (kernel_mem_pool -> size() == 0)
    {
        RSoD("Out of kernel memory.", faddr, _r);
    }

    // Currently, a page fault handler MAKES NO SENSE.
    RSoD("Kernel Segmentation Fault.", faddr, _r);

    /*size_t faddr_high = faddr & 0xFF300000;
    size_t faddr_med = faddr  & 0x003FF000;
    size_t faddr_low = faddr  & 0x00000FFF;

    // Retrieve the PageTable
    uint32_t * pde = &(current_paged_pd -> entries[faddr_high >> 22]);
    PTable * pt = (PTable*)(0xFF3 | faddr_med);
    if (!(*pde & PDirectory::PRESENT_MASK))
    {
        // If PageDirectoryEntry is not present, create PageTable
        size_t newpage = kernel_mem_pool -> next_frame();
        *pde = newpage | PDirectory::PRESENT_MASK;
        kernel_mem_pool -> alloc_mapped_frame();

        for (uint32_t eno = 0; eno < 1024; ++eno)
        {
            pt -> entries[eno] = 0;
        }
    }*/
    // Allocate targetted access page
}

static PDirectory * current_pd = NULL;

PDirectory * Memory::get_current_pd()
{
    return current_pd;
}

void Memory::set_current_pd(PDirectory * pd)
{
    current_pd = pd;
    if (pd != NULL)
    {
        __asm
        {
            mov eax, current_pd
            mov cr3, eax
        }
    }
    if (pd == NULL && current_pd != NULL)
    {
        // disable paging
        __asm
        {
            mov eax, cr0
            and eax, 0x7FFFFFFF
            mov cr0, eax
        }
    }
    else if (pd != NULL && current_pd == NULL)
    {
        // enable paging
        __asm
        {
            mov eax, cr0
            or eax, 0x80000000
            mov cr0, eax
        }
    }
}
