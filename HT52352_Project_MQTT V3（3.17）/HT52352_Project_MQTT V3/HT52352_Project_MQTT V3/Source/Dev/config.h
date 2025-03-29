#ifndef _CONFIG_H
#define _CONFIG_H


// MAIN_CONFIG宏	main.c 定义#define  MAIN_CONFIG
#ifdef 	 MAIN_CONFIG										// 头文件被多个C文件调用,避免变量冲突的问题 
	#define  EXT											// 主文件中定义的变量
#else
	#define  EXT	extern									// 全局文件变量的声明
#endif

//----------------------------------------		
//常用头文件包含
#include "ht32.h"  //芯片基础头文件
#include "ht32_board.h"
#include "stdio.h"
#include "string.h"
#include "math.h"
#include <stdarg.h> 
#include <stdbool.h>
#include <stdlib.h> 


//-------------------------------------
	
//外设驱动头文件包含
#include "system.h"  //系统相关头文件
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

//软件包头文件包含
#include "multi_timer.h"
#include "ringbuffer.h"


//----------------------------------------
//应用相关头文件包含
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

typedef enum //定义消息枚举类型
{
	MSG_NULL = 0,
	/******************************/
	//添加用户消息常量，例如：MSG_XXX,
	MSG_KEY1_PRESS,
	MSG_KEY2_PRESS,
	
	MSG_USART0_REC_FINISH,////串口usart0接收到一帧数据消息
	MSG_UART1_REC_FINISH,//串口uart1接收到一帧数据消息
	MSG_UART0_REC_FINISH,//串口uart0接收到一帧数据消息
	MSG_USART1_REC_FINISH,//串口usart1接收到一帧数据消息
	/******************************/
	MSG_NUM
	
}MSG_TYPE;

typedef struct 
{
	MSG_TYPE msg_type;            //系统消息类型，比如：MSG_UART1_REC_FINISH，代表串口uart1接收一帧数据结束
	unsigned char msg_parameter;  //系统消息参数，比如：当消息类型为MSG_UART1_REC_FINISH，msg_parameter代表帧长度
	
}system_message;



//----------------------------------------
EXT struct rt_ringbuffer msg_ring_buf;//存储消息的环形队列,在system.c中初始化

#endif

