#ifndef __UART0_H__
#define __UART0_H__

#include "ht32.h"

#define  UART0_RX_BUF_SIZE 256
#define  UART0_TXBUFF_SIZE   1024   //定义串口2 发送缓冲区大小 1024字节
extern struct rt_ringbuffer uart0_recv_ring_buf;//串口uart1接收环形队列声明

void Uart0_Init(int baudrate);
void Uart0_SendSring(char* str);
void UART0_printf(char*,...) ;
unsigned char Uart0_RecProcess(void);


#endif


