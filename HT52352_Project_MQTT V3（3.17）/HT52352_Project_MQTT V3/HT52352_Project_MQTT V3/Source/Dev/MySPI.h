#ifndef __MySPI_H__
#define __MySPI_H__	 

#include "ht32.h"

void MySPI_init(void);
void MySPI_Start(void);
void MySPI_Stop(void);
uint8_t MySPI_SwapByte(uint8_t ByteSend);

#endif

