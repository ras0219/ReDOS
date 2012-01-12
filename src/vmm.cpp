#include "vmm.h"

using namespace Memory;

PDT * Memory::init_physical_pdt(PhysFramePool * fp)
{
    // kernel page directory
    PDT * pdt = (PDT*) fp -> alloc_frame();

    // identity mapped region page table
    // (0 - 4 MB)
    PTEntry * pte = (PTEntry*) fp -> alloc_frame();

    for(size_t eno = 0; eno < PDT::NUM_ENTRIES; ++eno)
    {
        pdt -> entries[eno].entry = 0;
        pte[eno].entry = eno << 12
            | PTEntry::PRESENT_MASK;
    }
    pdt -> entries[0].entry = (uint32_t)pte | PDEntry::PRESENT_MASK;

    return pdt;
}
