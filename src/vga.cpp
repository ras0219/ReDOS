#include "stdinc.h"
#include "kernel.h"
#include "vga.h"

using namespace VGA;

void Console::init()
{
    x_pos = 0;
    y_pos = 0;
    attribs = attribs_stk;
    set_attribs(LIGHT_GREY, BLACK);
}

void Console::set_attribs(COLOR_CODE fg, COLOR_CODE bg)
{
	*attribs = (fg << 8) | (bg << 12);
}

void Console::push_attribs(COLOR_CODE fg, COLOR_CODE bg)
{
    ++attribs;
    if(attribs == attribs_stk + ATTRIB_STACK_SIZE)
    {
        --attribs;
        // ERROR! STACK OVERFLOW
        RSoD("ERROR, COLOR ATTRIB STACK OVERFLOW.",0xCAFEBABE);
    }
    set_attribs(fg, bg);
}
void Console::pop_attribs()
{
    --attribs;
    if(attribs < attribs_stk)
    {
        ++attribs;
        // ERROR! STACK UNDERFLOW
        RSoD("ERROR, COLOR ATTRIB STACK UNDERFLOW.",0xDEADBEEF);
    }
}

void Console::putc(const char c)
{
    _putc(c);
    _update_cursor();
}

void Console::_putc(const char c)
{
    bool clrln = false;
    switch (c)
    {
    case '\n':
        y_pos += 1;
        clrln = true;
        break;
    case '\r':
        x_pos = 0;
        break;
    case '\t':
        // 8-space tabs
        x_pos = ((x_pos >> 3) + 1) << 3;
        break;
    case '\b':
        x_pos -= 1;
        if (x_pos < 0)
        {
            y_pos -= 1;
            x_pos = CONSOLE_WIDTH - 1;
            if (y_pos < 0)
            {
                y_pos = 0;
                x_pos = 0;
            }
        }
	    CONSOLE_STARTADDR[x_pos + y_pos * CONSOLE_WIDTH] = *attribs;
        break;
    default:
	    CONSOLE_STARTADDR[x_pos + y_pos * CONSOLE_WIDTH] = c | *attribs;
	    x_pos += 1;
    }

	if (x_pos >= CONSOLE_WIDTH)
    {
		x_pos = 0;
		y_pos += 1;
        clrln = true;
    }
	if (y_pos >= CONSOLE_HEIGHT)
    {
		y_pos = 0;
	}
    if (clrln)
    {
        clearline(y_pos);
    }
}

void Console::puts(const char * s)
{
	while(*s != 0)
    {
		_putc(*s);
		++s;
	}
    _update_cursor();
}

void Console::clearline(uint8_t lno)
{
    for(int x=0;x<CONSOLE_WIDTH;x++)
    {
        CONSOLE_STARTADDR[x + lno * CONSOLE_WIDTH] = *attribs;
    }
}

void Console::clearscreen()
{
    for(uint8_t x=0;x<CONSOLE_HEIGHT;x++)
    {
        clearline(x);
    }
    x_pos = 0;
    y_pos = 0;
    _update_cursor();
}

void Console::_update_cursor()
{
    uint16_t temp = x_pos + y_pos * CONSOLE_WIDTH;
    outb(0x03D4, 0x0F);
    outb(0x03D5, temp & 0xFF);
    outb(0x03D4, 0x0E);
    outb(0x03D5, (temp>>8) & 0xFF);
}

Console& Console::operator<<(int32_t i)
{
    if (i < 0)
    {
        _putc('-');
        return operator<<((uint32_t)(-i));
    }
    return operator<<((uint32_t)i);
}
Console& Console::operator<<(uint32_t i)
{
    // maximum number of digits: 10
    char buf[11];
    char * curptr = buf+10;
    *curptr = 0;
    do
    {
        --curptr;
        *curptr = (i % 10) + '0';
        i /= 10;
    } while (i > 0);
    puts(curptr);
    return *this;
}
Console& Console::operator<<(int64_t i)
{
    if (i < 0)
    {
        _putc('-');
        return operator<<((uint64_t)(-i));
    }
    return operator<<((uint64_t)i);
}
Console& Console::operator<<(uint64_t i)
{
    static char hexchars[16] = {'0','1','2','3',
                                '4','5','6','7',
                                '8','9','A','B',
                                'C','D','E','F'};
    // maximum number of digits: 10
    char buf[19];
    char * curptr = buf+18;
    *curptr = 0;
    do
    {
        --curptr;
        *curptr = hexchars[i % 16];
        i >>= 4;
    } while (i > 0);
    --curptr;
    *curptr = 'x';
    --curptr;
    *curptr = '0';
    puts(curptr);
    return *this;
}

Console& Console::operator<<(REGS& r)
{
    *this << "GS: " << r.gs;
    *this << " FS: " << r.fs;
    *this << " ES: " << r.es;
    *this << " DS: " << r.ds << "\r\n";

    *this << "DI: " << r.edi;
    *this << " SI: " << r.esi;
    *this << " BP: " << r.ebp;
    *this << " SP: " << r.esp << "\r\n";

    *this << "BX: " << r.ebx;
    *this << " DX: " << r.edx;
    *this << " CX: " << r.ecx;
    *this << " AX: " << r.eax << "\r\n";

    *this << "INT_NO: " << r.int_no;
    *this << " ERR_CODE: " << r.err_code << "\r\n";

    *this << "EIP: " << (uint64_t)r.eip;
    *this << " CS: " << r.cs;
    *this << " EFLAGS: " << r.eflags;
    return *this;
}

void Console::_set_cursor_pos(uint8_t _x, uint8_t _y)
{
    x_pos = _x;
    y_pos = _y;
}
void Console::set_cursor_pos(uint8_t _x, uint8_t _y)
{
    _set_cursor_pos(_x, _y);
    _update_cursor();
}