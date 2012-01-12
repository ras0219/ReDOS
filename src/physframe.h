#ifndef _PHYSFRAME_H_
#define _PHYSFRAME_H_

#include "stdinc.h"

namespace Memory
{
    struct PhysFrame;
    struct PhysFramePool
    {
        PhysFrame * first;
        PhysFrame * last;
        uint32_t num_frames;

        void append_pool(PhysFramePool * p);
        inline size_t alloc_frame() { return (size_t)_alloc_frame(); }
        inline void init_pool(size_t start, size_t end) {
            _init_pool((PhysFrame*)start,(PhysFrame*)end); }
        inline uint32_t size() { return num_frames; }
        inline size_t alloc_mapped_frame(size_t logical_addr) {
            return (size_t)_alloc_mapped_frame((PhysFrame*)logical_addr); }
        inline size_t next_frame() { return (size_t)_next_frame(); }
        inline void dealloc_frame(size_t f) {
            _dealloc_frame((PhysFrame*)f); }

        void _init_pool(PhysFrame * start, PhysFrame * end);
        PhysFrame * _alloc_frame();
        inline PhysFrame * _next_frame() { return first; }
        PhysFrame * _alloc_mapped_frame(PhysFrame * logical_addr);
        void _dealloc_frame(PhysFrame * f);
    };
}

#endif
