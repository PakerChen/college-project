#ifndef __SYSTICK_H__
#define __SYSTICK_H__

#include "ht32.h"


#define  MAX_TIMER_SYSTICK  		5						//���ʱ������

extern unsigned long sysTimer[MAX_TIMER_SYSTICK ];		//����ϵͳ��ʱ�� 
#define  DelayTimer   	sysTimer[0]	 		//��ʱ��ʱ��
#define  Uart1RecTimer 	sysTimer[1]  //����uart1�ַ����ն�ʱ���������ʱ����ʾһ֡�������
#define	 Usart0RecTimer sysTimer[2]  //����usart0�ַ����ն�ʱ���������ʱ����ʾһ֡�������
#define  Uart0RecTimer 	sysTimer[3]  //����uart1�ַ����ն�ʱ���������ʱ����ʾһ֡�������
#define  Usart1RecTimer	sysTimer[4]

void SysTickInit(void);	


void SysTickDelayMs(unsigned long  nms);
void SysTickDelayUs(unsigned long  nus);

unsigned long  GetRunTime(void);

#endif
/***************************************************************************************
***************************************end of file**************************************
***************************************************************************************/
