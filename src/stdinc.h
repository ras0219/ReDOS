#ifndef _STDINC_H_
#define _STDINC_H_

#define NULL 0
// debug mode
#define REDOS_DEBUG

#pragma warning(disable:4127)
#pragma code_seg(".text$5")

#include "types.h"

extern "C" int32_t _outp(uint16_t, int32_t);
extern "C" uint16_t _outpw(uint16_t, uint16_t);
extern "C" int32_t _inp(uint16_t);
extern "C" uint16_t _inpw(uint16_t);
extern "C" void * _ReturnAddress();
extern "C" void __halt();
extern "C" void __lidt(void * Source);
extern "C" long _InterlockedExchange(uint32_t * target, uint32_t val);
 
#define outb(prt, val)  _outp(prt, val)
#define inb(prt)        ((uint8_t)_inp(prt))
#define outw(prt, val)  _outpw(prt, val)
#define inw(prt)        ((uint16_t)_inpw(prt))
#define getRetAddr      _ReturnAddress
#pragma intrinsic(_ReturnAddress, _outp, _inp, \
                  _outpw, _inpw, __halt, __lidt, \
                  _InterlockedExchange)

#endif