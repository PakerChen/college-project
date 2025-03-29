#ifndef __USART1_H__
#define __USART1_H__

#include "ht32.h"

#define  USART1_RX_BUF_SIZE 256
#define  USART1_TXBUFF_SIZE   1024   //定义串口2 发送缓冲区大小 1024字节
extern struct rt_ringbuffer usart1_recv_ring_buf;//串口uart1接收环形队列声明

void Usart1_Init(int baudrate);
void Usart1_SendSring(char* str);
void USART1_printf(char*,...) ;
unsigned char Usart1_RecProcess(void);


#endif
