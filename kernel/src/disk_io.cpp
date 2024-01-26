#include "disk_io.h"
#include "pci_io.h"

void diskInit() 
{
    pciInit();
}

bool diskMount(const char* partition, char letter)
{
    return false;
}

bool fileOpen(const char* fileName)
{
    return false;
}

void fileSeek(bool current, uint32_t offset)
{

}

void fileRead(uint8_t* buffer, uint32_t amount)
{

}

uint32_t fileSize()
{
    return 0;
}
