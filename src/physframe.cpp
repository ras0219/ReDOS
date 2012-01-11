#include "physframe.h"

using namespace Memory;

void PhysFramePool::init_pool(PhysFrame * start, PhysFrame * end)
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
    while (start + 1 != end)
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
    last -> next_free = p -> first;
    last = p -> last;
    num_frames += p -> num_frames;

    p -> first = NULL;
    p -> last = NULL;
    p -> num_frames = 0;
}

PhysFrame * PhysFramePool::alloc_frame()
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

void PhysFramePool::dealloc_frame(PhysFrame * f)
{
#ifdef _DEBUG
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
