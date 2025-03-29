#ifndef __source_H__
#define __source_H__

#include "ht32.h"
void SourceInit(void);
int SourceWriteByte(unsigned char val);
int SourceReadByte(unsigned char *val);

#define MCP4017_READ_ADDR    0x5F
#define MCP4017_WRITE_ADDR   0x5E
#endif

