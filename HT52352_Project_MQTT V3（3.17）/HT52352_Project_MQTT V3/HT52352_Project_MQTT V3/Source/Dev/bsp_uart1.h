#ifndef __UART1_H__
#define __UART1_H__

#include "ht32.h"

#define  UART1_RX_BUF_SIZE 256
extern uint8_t 	s_UART1_RecFrameLen;
extern struct rt_ringbuffer uart1_recv_ring_buf;//串口uart1接收环形队列声明

void Uart1_Init(int baudrate);
void Uart1_SendSring(char* str);
unsigned char Uart1_RecProcess(void);


#endif
