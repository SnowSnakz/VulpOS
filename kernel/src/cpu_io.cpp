#include "cpu_io.h"

void __cpu_outb(uint16_t port, uint8_t val) {
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

void __cpu_outw(uint16_t port, uint16_t val) {
    asm volatile ("outw %0, %1" : : "a"(val), "Nd"(port));
}

void __cpu_outl(uint16_t port, uint32_t val) {
    asm volatile ("outl %0, %1" : : "a"(val), "Nd"(port));
}

uint8_t __cpu_inb(uint16_t port) {
    uint8_t result;
    asm volatile ("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

uint16_t __cpu_inw(uint16_t port) {
    uint16_t result;
    asm volatile ("inw %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

uint32_t __cpu_inl(uint16_t port) {
    uint32_t result;
    asm volatile ("inl %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

void halt()
{
    // Enter infinite loop.
    for(;;);

    // Fallback on CPU halt.
    asm volatile("hlt");
}

uint32_t __cpu_apic_id() 
{
    uint32_t apic_id;
    asm volatile (
        "mov $1, %%eax\n"
        "cpuid\n"
        "shr $24, %%ebx\n"
        "mov %%ebx, %0\n"
        : "=r" (apic_id)  // Output operand
        :                 // Input operand
        : "%eax", "%ebx", "%ecx", "%edx"  // Clobbered registers
    );
    return apic_id;
}
