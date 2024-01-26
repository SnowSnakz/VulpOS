#include "cpu_io.h"
#include "keyboard_io.h"

void kbWait() 
{
    while ((__cpu_inb(0x64) & 0x02) != 0);
}

uint8_t kbRead()
{
    kbWait();
    return __cpu_inb(0x60);
}
