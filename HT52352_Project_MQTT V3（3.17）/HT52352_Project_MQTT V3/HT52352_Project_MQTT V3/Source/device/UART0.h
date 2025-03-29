#ifndef __UART0_H__
#define __UART0_H__

#include "ht32.h"

#define  UART0_RX_BUF_SIZE 256
#define  UART0_TXBUFF_SIZE   1024   //���崮��2 ���ͻ�������С 1024�ֽ�
extern struct rt_ringbuffer uart0_recv_ring_buf;//����uart1���ջ��ζ�������

void Uart0_Init(int baudrate);
void Uart0_SendSring(char* str);
void UART0_printf(char*,...) ;
unsigned char Uart0_RecProcess(void);


#endif


