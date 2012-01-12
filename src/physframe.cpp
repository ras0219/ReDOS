#include "physframe.h"

using namespace Memory;

struct Memory::PhysFrame
{
    static const size_t FRAME_SIZE = 0x1000;
    static const size_t PADDING_SIZE = FRAME_SIZE - sizeof(PhysFrame*);
    static const size_t NUM_ENTRIES = PADDING_SIZE / sizeof(uint32_t);
    PhysFrame * next_free;
    uint32_t padding[NUM_ENTRIES];
};

void PhysFramePool::_init_pool(PhysFrame * start, PhysFrame * end)
{
    if (start >= end)
    {
        first = NULL;
        last = NULL;
        num_frames = 0;
        return;
    }
    first = start;
    last = end - 1;
    while (start + 1 < end)
    {
        start -> next_free = start + 1;
        ++start;
    }
    start -> next_free = NULL;
}

void PhysFramePool::append_pool(PhysFramePool * p)
{
    if (p->num_frames == 0)
    {
        return;
    }
    if (num_frames == 0)
    {
        first = p -> first;
    }
    else
    {
        last -> next_free = p -> first;
    }
    last = p -> last;
    num_frames += p -> num_frames;

    p -> first = NULL;
    p -> last = NULL;
    p -> num_frames = 0;
}

PhysFrame * PhysFramePool::_alloc_frame()
{
    if (num_frames == 0)
    {
        return NULL;
    }

    --num_frames;
    if (num_frames == 0)
    {
        last = NULL;
    }
    PhysFrame * ret = first;
    first = first -> next_free;
    return ret;
}

PhysFrame * PhysFramePool::_alloc_mapped_frame(PhysFrame * logical_addr)
{
    if (num_frames == 0)
    {
        return NULL;
    }

    --num_frames;
    if (num_frames == 0)
    {
        last = NULL;
    }
    PhysFrame * ret = first;
    first = first -> next_free;
    return ret;
}

void PhysFramePool::_dealloc_frame(PhysFrame * f)
{
#ifdef REDOS_DEBUG
    if (f == NULL)
    {
        return;
    }
#endif
    if (num_frames == 0)
    {
        last = f;
    }
    ++num_frames;
    f -> next_free = first;
    first = f;
}
