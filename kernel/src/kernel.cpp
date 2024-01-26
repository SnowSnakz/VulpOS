#include <bootboot.h>

#include "cpu_io.h"
#include "pci_io.h"
#include "disk_io.h"
#include "console_io.h"
#include "keyboard_io.h"

extern BOOTBOOT bootboot;
extern unsigned char environment[4096];
extern uint8_t fb;

void filePrint();
void error(const char* message);

void loadOS();
void waitForOS();

int main()
{
    if(__cpu_apic_id() == 0)
    {
        loadOS();
    }
    else
    {
        waitForOS();
    }

    return 0;
}

void filePrint()
{
    char buffer[256];
    uint32_t length = fileSize(), amount;
    while(length > 0) 
    {
        amount = length;
        if(length > 254)
            amount = 254;

        length -= amount;

        fileRead((uint8_t*)buffer, amount);
        buffer[amount + 1] = 0;

        termWrite((const char*)buffer);
    }
}

void error(const char* message)
{
    const uint32_t color = 0xFFAD0000;

    termColor(color, 0xFF000000);
    termWriteLine(message);

    termColor(0xFFFFFFFF, 0xFF000000);
}

void hexPrint(uint8_t value)
{
    const char* decToHex = "0123456789ABCDEF";

    termWrite(decToHex[(value & 0xF0) >> 4]);
    termWrite(decToHex[value & 0x0F]);
}

void hexPrint(uint16_t value)
{
    hexPrint((uint8_t)((value & 0xFF00) >> 8));
    hexPrint((uint8_t)(value & 0xFF));
}

void hexPrint(uint32_t value)
{
    hexPrint((uint16_t)((value & 0xFFFF0000) >> 16));
    hexPrint((uint16_t)(value & 0x0000FFFF));
}

void loadOS()
{
    // Initialize Console I/O
    termInit((uint32_t*)(&fb));
    termClear(0xFF000000);

    // Initialize PCI I/O
    pciInit();

    // Display PCI info.
    uint32_t count = pciCount();

    termWrite("PCI Count: 0x");
    hexPrint(count);
    termWriteLine();

    for(uint32_t i = 0; i < count; i++)
    {
        PCIdevice device = pciFind(i);

        termWrite("=== DEVICE 0x");
        hexPrint(i);
        termWriteLine(" ===");

        termWrite("Vendor: ");
        hexPrint(device.vendor);
        termWrite("  |  ");
        
        termWrite("Device: ");
        hexPrint(device.device);
        termWrite("  |  ");
        
        termWrite("Function: ");
        hexPrint(device.function);
        termWriteLine();
    }

    // Initialize Disk I/O
    diskInit();

    // Mount Root and User Partitions
    if(!diskMount("4F68BCE3-E8CD-4DB1-96E7-FBCAF984B709", 'R'))
    {
        error("Unable to mount root fs.");
        halt();
    }

    if(!diskMount("8484680C-9521-48C6-9C11-B0720656F69E", 'U'))
    {
        error("Unable to mount user fs.");
        halt();
    }

    // As a test, read happy.txt and path_test.txt from root.
    termColor(0xFF42F587, 0xFF000000);
    termWriteLine("--- From 'R:/happy.txt' ---");

    if(fileOpen("R:/happy.txt"))
        filePrint();
    else
        termWrite("Unable to open file...");

    termWriteLine();

    termColor(0xFF7B42F5, 0xFF000000);
    termWriteLine("--- From 'R:/System/path_test.txt' ---");

    if(fileOpen("R:/System/path_test.txt"))
        filePrint();
    else
        termWrite("Unable to open file...");

    // As a test, read super.txt from user.
    termColor(0xFFF58142, 0xFF000000);
    termWriteLine("--- From 'U:/Common/super.txt' ---");
    
    if(fileOpen("U:/Common/super.txt"))
        filePrint();
    else
        termWrite("Unable to open file...");

    // Halt.
    halt();
}

void waitForOS()
{
    halt();
}
