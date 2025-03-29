#ifndef __USART0_H
#define __USART0_H

#include "ht32.h"
#include "stdio.h"      //������Ҫ��ͷ�ļ�
#include "stdarg.h"		//������Ҫ��ͷ�ļ� 
#include "string.h"     //������Ҫ��ͷ�ļ�
  
			
#define USART0_RX_ENABLE     1      //�Ƿ������չ���  1������  0���ر�
#define USART0_TXBUFF_SIZE   1024   //���崮��2 ���ͻ�������С 1024�ֽ�

#if  USART0_RX_ENABLE                          //���ʹ�ܽ��չ���
#define USART0_RXBUFF_SIZE   1024              //���崮��2 ���ջ�������С 1024�ֽ�
extern char Usart0_RxCompleted ;               //�ⲿ�����������ļ����Ե��øñ���
extern uint16_t Usart0_RxCounter;          //�ⲿ�����������ļ����Ե��øñ���
extern char Usart0_RxBuff[USART0_RXBUFF_SIZE]; //�ⲿ�����������ļ����Ե��øñ���
#endif

extern char DMA_flag;

void Usart0_Init(int baudrate);       
void Usart0_IDELInit(int baudrate);
void u0_printf(char*,...) ;          
void u0_TxData(unsigned char *data);
void Usart0DMA_init(void);
unsigned char Usart0_RecProcess(void);

#endif
