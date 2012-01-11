#ifndef _VGA_H_
#define _VGA_H_

#include "types.h"
#include "machine.h"

#pragma warning(disable:4480)

namespace VGA {

    static uint16_t * const CONSOLE_STARTADDR = (uint16_t*)0xB8000;
	static const uint8_t CONSOLE_WIDTH = 80;
	static const uint8_t CONSOLE_HEIGHT = 25;

    enum COLOR_CODE :uint8_t {
	    BLACK         = 0,
	    BLUE          = 1,
	    GREEN         = 2,
	    CYAN          = 3,
	    RED	          = 4,
	    MAGENTA       = 5,
	    BROWN         = 6,
	    LIGHT_GREY    = 7,
	    DARK_GREY     = 8,
	    LIGHT_BLUE    = 9,
	    LIGHT_GREEN   = 10,
	    LIGHT_CYAN    = 11,
	    LIGHT_RED     = 12,
	    LIGHT_MAGENTA = 13,
	    LIGHT_BROWN   = 14,
	    WHITE	      = 15
    };

    struct Console {
        static const size_t ATTRIB_STACK_SIZE = 16;
        uint8_t x_pos;
        uint8_t y_pos;
        // Stack of 16 color combinations
        uint16_t attribs_stk[ATTRIB_STACK_SIZE];
        uint16_t * attribs;

        void init();
        void putc(const char c);
	    void puts(const char * s);
	    void set_attribs(COLOR_CODE fg, COLOR_CODE bg);
        void push_attribs(COLOR_CODE fg, COLOR_CODE bg);
        void pop_attribs();
        void clearline(uint8_t lno);
        void clearscreen();
        void _update_cursor();
        void _putc(const char c);
        void set_cursor_pos(uint8_t _x, uint8_t _y);
        void _set_cursor_pos(uint8_t _x, uint8_t _y);
        
        inline Console& operator<<(const char c) { putc(c); return *this; }
        inline Console& operator<<(const char * s) { puts(s); return *this; }
        
        Console& operator<<(int32_t i);
        Console& operator<<(uint32_t i);
        Console& operator<<(int64_t i);
        Console& operator<<(uint64_t i);
        Console& operator<<(REGS& r);
//        Console& operator<<(void (*f)(Console&));
    };
}
#endif