#ifndef PCI_IO_H
#define PCI_IO_H

#include "inttypes.h"

struct PCIdevice
{
    uint32_t vendor;
    uint32_t device;
    uint32_t function;
};

void pciInit();

uint32_t pciCount();
PCIdevice pciFind(uint32_t index);

#endif
