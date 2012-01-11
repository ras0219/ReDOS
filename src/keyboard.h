#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "stdinc.h"
#include "machine.h"

namespace Devices
{
    namespace Keyboard
    {
        uint32_t keyboard_isr(REGS * _r);

        void async_register(void (*func)(char c));
        char getc();
        const char * readline();
    }
}

#endif