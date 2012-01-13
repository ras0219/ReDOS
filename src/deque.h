#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "stdinc.h"

namespace rstd
{
    template<typename T, size_t _sz>
    struct FixedRingDeque
    {
        static T FAILURE = NULL;
        T ring[_sz];
        size_t front;
        size_t back;
        size_t num;

        void init()
        {
            front = 0;
            back = _sz - 1;
            num = 0;
        }
        uint32_t push_front(T& elem)
        {
            if (num == _sz)
            {
                return 1;
            }
            ++num;
            ring[front] = elem;
            front = (front + 1) % _sz;
            return 0;
        }
        uint32_t push_back(T& elem)
        {
            if (num == _sz)
            {
                return 1;
            }
            ++num;
            ring[back] = elem;
            back = (back + _sz - 1) % _sz;
            return 0;
        }
        T& pop_front()
        {
            if (num == 0)
            {
                return FAILURE;
            }
            --num;
            front = (front + _sz - 1) % _sz;
            return ring[front];
        }
        T& pop_back()
        {
            if (num == 0)
            {
                return FAILURE;
            }
            --num;
            back = (back + 1) % _sz;
            return ring[back];
        }
        T& peek_front()
        {
            if (num == 0)
            {
                return FAILURE;
            }
            return ring[(front + 1) % _sz];
        }
        T& peek_back()
        {
            if (num == 0)
            {
                return FAILURE;
            }
            return ring[(back + 1) % _sz];
        }
        size_t size()
        {
            return num;
        }
        size_t capacity()
        {
            return _sz;
        }
    };
}

#endif