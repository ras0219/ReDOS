#ifndef _IRQ_H_
#define _IRQ_H_

#include "stdinc.h"

namespace IRQ
{
    void remap_pics();

    void mask(uint8_t irq);
    void unmask(uint8_t irq);
}

#endif