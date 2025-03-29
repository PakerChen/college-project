#ifndef __USART0_H
#define __USART0_H

#include "ht32.h"
#include "stdio.h"      //包含需要的头文件
#include "stdarg.h"		//包含需要的头文件 
#include "string.h"     //包含需要的头文件
  
			
#define USART0_RX_ENABLE     1      //是否开启接收功能  1：开启  0：关闭
#define USART0_TXBUFF_SIZE   1024   //定义串口2 发送缓冲区大小 1024字节

#if  USART0_RX_ENABLE                          //如果使能接收功能
#define USART0_RXBUFF_SIZE   1024              //定义串口2 接收缓冲区大小 1024字节
extern char Usart0_RxCompleted ;               //外部声明，其他文件可以调用该变量
extern uint16_t Usart0_RxCounter;          //外部声明，其他文件可以调用该变量
extern char Usart0_RxBuff[USART0_RXBUFF_SIZE]; //外部声明，其他文件可以调用该变量
#endif

extern char DMA_flag;

void Usart0_Init(int baudrate);       
void Usart0_IDELInit(int baudrate);
void u0_printf(char*,...) ;          
void u0_TxData(unsigned char *data);
void Usart0DMA_init(void);
unsigned char Usart0_RecProcess(void);

#endif
