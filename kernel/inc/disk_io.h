#ifndef DISK_IO_H
#define DISK_IO_H

#include "inttypes.h"


void diskInit();
bool diskMount(const char* partition, char letter);

bool fileOpen(const char* fileName);
void fileSeek(bool current, uint32_t offset);
void fileRead(uint8_t* buffer, uint32_t amount);
uint32_t fileSize();

#endif
