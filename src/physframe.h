#ifndef _PHYSFRAME_H_
#define _PHYSFRAME_H_

#include "stdinc.h"

namespace Memory
{
    struct PhysFrame
    {
        static const size_t NUM_ENTRIES = 1024;
        PhysFrame * next_free;
        uint32_t padding[NUM_ENTRIES - 1];
    };
    struct PhysFramePool
    {
        PhysFrame * first;
        PhysFrame * last;
        uint32_t num_frames;

        void init_pool(PhysFrame * start, PhysFrame * end);
        void append_pool(PhysFramePool * p);
        PhysFrame * alloc_frame();
        void dealloc_frame(PhysFrame * f);
        inline uint32_t size()
        {
            return num_frames;
        }
    };
}

#endif