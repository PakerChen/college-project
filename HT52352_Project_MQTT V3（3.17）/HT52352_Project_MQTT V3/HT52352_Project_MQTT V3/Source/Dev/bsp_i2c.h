#ifndef _I2C_H
#define _I2C_H
#include "ht32.h"

void I2c_GpioInit(void);
void I2cStart(void);
void I2cStop( void );
void I2cACK(void);
void I2cNoACK(void);
int I2cRACK(void);
void I2cWriteByte(unsigned char b);
void I2cReadByte(unsigned char *dat);


#endif



