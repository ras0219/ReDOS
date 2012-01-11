#include "keyboard.h"

using namespace Devices;

static void (*async_func)(char c) = NULL;
static uint32_t lastchar = NULL;
static uint8_t shifted = 0;
static uint8_t escaped = 0;
static const char keytrans_shifted[128] = { 0 };
static const char keytrans[128] = {
0x00, 0x1B, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, // 00-07
0x37, 0x38, 0x39, 0x30, 0x2D, 0x3D, 0x08, 0x09, // 08-0F

0x71, 0x77, 0x65, 0x72, 0x74, 0x79, 0x75, 0x69, // 10-17
0x6F, 0x70, 0x5B, 0x5D, 0x0A, 0x00, 0x61, 0x73, // 18-1F

0x64, 0x66, 0x67, 0x68, 0x6A, 0x6B, 0x6C, 0x3B, // 20-27
0x27, 0x60, 0x00, 0x5C, 0x7A, 0x78, 0x63, 0x76, // 28-2F

0x62, 0x6E, 0x6D, 0x2C, 0x2E, 0x2F, 0x00, 0x00, // 30-37
0x00, 0x20, 0x00
};

char Keyboard::getc()
{
    uint32_t charbuf = NULL;
    while (charbuf == NULL)
    {
        charbuf = _InterlockedExchange(&lastchar, NULL);
    }
    return (char) charbuf;
}

uint32_t Keyboard::keyboard_isr(REGS * _r)
{
    uint32_t sc = inb(0x60);
    if (escaped)
    {
        //sc += 256;
        // currently not worrying about escaping
        escaped = 0;
    }
    switch (sc)
    {
    case 0x00:
        // keyboard error
        return 1;
    case 0x2A:
        shifted = 1;
        return 0;
    case 0xAA:
        shifted = 0;
        return 0;
    case 0xE0:
        escaped = 1;
        return 0;
    default:
        if (sc & 0x80)
        {
            // do nothing, it's a break
            return 0;
        }
        else
        {
            lastchar = (shifted?keytrans_shifted:keytrans)[sc];
        }
    }
    return 0;
}

void Keyboard::async_register(void (*func)(char c))
{
    // currently unused
}
