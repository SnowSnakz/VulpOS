#ifndef CONSOLE_IO_H
#define CONSOLE_IO_H

#include "inttypes.h"

void termInit(uint32_t* framebuffer);

void termClear(uint32_t background);
void termColor(uint32_t foreground, uint32_t background);

void termPush();

void termWrite(const char* str);
void termWrite(char c);

void termWriteLine();
void termWriteLine(const char* str);

void termSetTabSize(uint32_t size);
void termSetTextWrap(bool wrap);

#endif
