#ifndef _CONFIG_H
#define _CONFIG_H


// MAIN_CONFIG��	main.c ����#define  MAIN_CONFIG
#ifdef 	 MAIN_CONFIG										// ͷ�ļ������C�ļ�����,���������ͻ������ 
	#define  EXT											// ���ļ��ж���ı���
#else
	#define  EXT	extern									// ȫ���ļ�����������
#endif

//----------------------------------------		
//����ͷ�ļ�����
#include "ht32.h"  //оƬ����ͷ�ļ�
#include "ht32_board.h"
#include "stdio.h"
#include "string.h"
#include "math.h"
#include <stdarg.h> 
#include <stdbool.h>
#include <stdlib.h> 


//-------------------------------------
	
//��������ͷ�ļ�����
#include "system.h"  //ϵͳ���ͷ�ļ�
#include "bsp_Systick.h"
#include "bsp_usart0.h"
#include "bsp_uart1.h"
#include "bsp_i2c.h"
#include "bsp_ATC02.h"
#include "bsp_exti.h"
#include "bsp_BFTM.h"
#include "bsp_18b20.h"
#include "bsp_pwm.h"
#include "bsp_GPTM.h"
#include "bsp_wifi.h"	
#include "mqtt.h"
#include "utils_hmac.h" 
//----------------------------------------	

//�����ͷ�ļ�����
#include "multi_timer.h"
#include "ringbuffer.h"


//----------------------------------------
//Ӧ�����ͷ�ļ�����
#include "task.h"
//----------------------------------------
#include "UART0.h"
#include "USART1.h"
#include "source.h"
#include "SYSTimer.h"
#include "INA226_IIC.h"
#include "INA226.h"
#include "iic.h"
#include "MySPI.h"
#include "W25Q64.h"

typedef enum //������Ϣö������
{
	MSG_NULL = 0,
	/******************************/
	//����û���Ϣ���������磺MSG_XXX,
	MSG_KEY1_PRESS,
	MSG_KEY2_PRESS,
	
	MSG_USART0_REC_FINISH,////����usart0���յ�һ֡������Ϣ
	MSG_UART1_REC_FINISH,//����uart1���յ�һ֡������Ϣ
	MSG_UART0_REC_FINISH,//����uart0���յ�һ֡������Ϣ
	MSG_USART1_REC_FINISH,//����usart1���յ�һ֡������Ϣ
	/******************************/
	MSG_NUM
	
}MSG_TYPE;

typedef struct 
{
	MSG_TYPE msg_type;            //ϵͳ��Ϣ���ͣ����磺MSG_UART1_REC_FINISH��������uart1����һ֡���ݽ���
	unsigned char msg_parameter;  //ϵͳ��Ϣ���������磺����Ϣ����ΪMSG_UART1_REC_FINISH��msg_parameter����֡����
	
}system_message;



//----------------------------------------
EXT struct rt_ringbuffer msg_ring_buf;//�洢��Ϣ�Ļ��ζ���,��system.c�г�ʼ��

#endif

