#ifndef _STDIO_H_
#define _STDIO_H_

#include "stdinc.h"
#include "vga.h"
#include "keyboard.h"

#define assert(X) \
    if (!(X)) \
    { \
        cnsl << #X << __LINE__ << __FILE__;\
        __halt(); \
    }

extern VGA::Console cnsl;

#endif