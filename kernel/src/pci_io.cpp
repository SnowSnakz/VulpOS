#include "pci_io.h"
#include "cpu_io.h"

bool pciIsReady = false;

uint16_t pciRead(uint32_t bus, uint32_t slot, uint32_t func, uint32_t offset)
{
	uint32_t address = (bus << 16) | (slot << 11) | (func << 8) | (offset & 0xFC) | 0x80000000;
    __cpu_outl (0xCF8, address);

    return (uint16_t)((__cpu_inl(0xCFC) >> ((offset & 2) << 3)) & 0xFFFF);
}

uint16_t getVendorID(uint16_t bus, uint16_t device, uint16_t function)
{
        uint32_t r0 = pciRead(bus, device, function, 0x0);
        return r0;
}

uint16_t getDeviceID(uint16_t bus, uint16_t device, uint16_t function)
{
        uint32_t r0 = pciRead(bus, device, function, 0x2);
        return r0;
}

uint16_t getClassId(uint16_t bus, uint16_t device, uint16_t function)
{
        uint32_t r0 = pciRead(bus, device, function, 0xA);
        return (r0 >> 8) & 0xFF;
}

uint16_t getSubClassId(uint16_t bus, uint16_t device, uint16_t function)
{
        uint32_t r0 = pciRead(bus, device, function, 0xA);
        return r0 & 0xFF;
}

PCIdevice __pci_devices[256];
uint16_t __pci_device_count;

void pciInit() 
{
    // Don't do anything if this method has already been successfully called.
    if(pciIsReady)
        return;

    // Allocate temporary memory for PCIdevice data during iteration.
    PCIdevice dev;

    // Iterate PCI devices with Brute Force 
    for(uint32_t bus = 0; bus < 256; bus++)
    {
        // Only 255 active devices supported at once.
        if(__pci_device_count > 255)
            break;
            
        for(uint32_t slot = 0; slot < 32; slot++)
        {
            // Only 255 active devices supported at once.
            if(__pci_device_count > 255)
                break;
            
            for(uint32_t function = 0; function < 8; function++)
            {
                // Only 255 active devices supported at once.
                if(__pci_device_count > 255)
                    break;

                // Get the device vendor.
                uint16_t vendor = getVendorID(bus, slot, function);
                if(vendor == 0xFFFF) continue;

                // Get the device id.
                uint16_t device = getDeviceID(bus, slot, function);
                
                // Set PCIdevice data.
                dev.vendor = vendor;
                dev.device = device;
                dev.function = function;

                // Add PCIdevice to active devices.
                __pci_devices[__pci_device_count] = dev;
                __pci_device_count++;
            }
        }
    }

    // Allow other PCI functions to be called.
    pciIsReady = true;
}

uint32_t pciCount()
{
    return __pci_device_count;
}

PCIdevice pciFind(uint32_t index)
{
    return __pci_devices[index & 0xFF];
}
