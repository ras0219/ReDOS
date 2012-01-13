#ifndef _THREAD_H_
#define _THREAD_H_

#include "stdinc.h"
#include "deque.h"

namespace Thread
{
    struct Thread
    {
        size_t esp;
    };

    struct ThreadScheduler
    {
        typedef rstd::FixedRingDeque<Thread*,10> queue_type;
        Thread * current_th;
        queue_type tq;

        void init();
        void yield();
        void enqueue(Thread * t);
    };
}

#endif