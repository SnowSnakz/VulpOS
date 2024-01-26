#ifndef CPU_H
#define CPU_H

#include "inttypes.h"

void __cpu_outb(uint16_t port, uint8_t  val);
void __cpu_outw(uint16_t port, uint16_t val);
void __cpu_outl(uint16_t port, uint32_t val);

uint8_t  __cpu_inb(uint16_t port);
uint16_t __cpu_inw(uint16_t port);
uint32_t __cpu_inl(uint16_t port);

uint32_t __cpu_apic_id();

void halt();

#endif
