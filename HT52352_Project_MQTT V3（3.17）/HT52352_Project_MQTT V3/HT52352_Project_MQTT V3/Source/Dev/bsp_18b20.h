#ifndef _18B20_H
#define _18B20_H
#include "config.h"
void DS18B20_IO_IN(void);
void DS18B20_IO_OUT(void);

#define GPIO_DS18B20 HT_GPIOA
#define IO_Temp  GPIO_PIN_1

//IO操作
#define	DS18B20_DQ_OUT_H HT_GPIOA->SRR = GPIO_PIN_1
#define	DS18B20_DQ_OUT_L HT_GPIOA->RR = GPIO_PIN_1
#define	DS18B20_DQ_IN  GPIO_ReadInBit(HT_GPIOA,GPIO_PIN_1)

double DS18B20_Get_Temp(void);//获取温度

void DS18B20_Rst(void);//复位DS18B20    
void Sket_Delayus(u32 nus);
void Sket_Delayms(u16 nms);
int db18b20_check(void);
#endif

