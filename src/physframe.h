#ifndef _PHYSFRAME_H_
#define _PHYSFRAME_H_

#include "stdinc.h"

namespace Memory
{
    struct PhysFrame
    {
        static const size_t FRAME_SIZE = 0x1000;
        static const size_t PADDING_SIZE = FRAME_SIZE - sizeof(PhysFrame*);
        static const size_t NUM_ENTRIES = PADDING_SIZE / sizeof(uint32_t);
        PhysFrame * next_free;
        uint32_t padding[NUM_ENTRIES];
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