#ifndef __SYSTICK_H__
#define __SYSTICK_H__

#include "ht32.h"


#define  MAX_TIMER_SYSTICK  		5						//最大定时器个数

extern unsigned long sysTimer[MAX_TIMER_SYSTICK ];		//定义系统定时器 
#define  DelayTimer   	sysTimer[0]	 		//延时定时器
#define  Uart1RecTimer 	sysTimer[1]  //串口uart1字符接收定时器，如果超时，表示一帧接收完毕
#define	 Usart0RecTimer sysTimer[2]  //串口usart0字符接收定时器，如果超时，表示一帧接收完毕
#define  Uart0RecTimer 	sysTimer[3]  //串口uart1字符接收定时器，如果超时，表示一帧接收完毕
#define  Usart1RecTimer	sysTimer[4]

void SysTickInit(void);	


void SysTickDelayMs(unsigned long  nms);
void SysTickDelayUs(unsigned long  nus);

unsigned long  GetRunTime(void);

#endif
/***************************************************************************************
***************************************end of file**************************************
***************************************************************************************/
