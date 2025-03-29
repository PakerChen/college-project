#ifndef __USART1_H__
#define __USART1_H__

#include "ht32.h"

#define  USART1_RX_BUF_SIZE 256
#define  USART1_TXBUFF_SIZE   1024   //���崮��2 ���ͻ�������С 1024�ֽ�
extern struct rt_ringbuffer usart1_recv_ring_buf;//����uart1���ջ��ζ�������

void Usart1_Init(int baudrate);
void Usart1_SendSring(char* str);
void USART1_printf(char*,...) ;
unsigned char Usart1_RecProcess(void);


#endif
