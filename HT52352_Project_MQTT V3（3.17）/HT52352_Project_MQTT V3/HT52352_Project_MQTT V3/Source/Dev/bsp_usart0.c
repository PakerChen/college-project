#include "config.h"
#include "bsp_usart0.h"


#if  USART0_RX_ENABLE                   //如果使能接收功能
char Usart0_RxCompleted = 0;            //定义一个变量 0：表示接收未完成 1：表示接收完成 
uint16_t Usart0_RxCounter = 0;      //定义一个变量，记录串口2总共接收了多少字节的数据
char Usart0_RxBuff[USART0_RXBUFF_SIZE]; //定义一个数组，用于保存串口2接收到的数据   	
#endif

char DMA_flag = 0;                      // 0 DMA发送空闲   1 DMA发


/*********************************************************************************************************//**
  * @brief Usart0_Init  Usart0_Init
  * @param  baudrate :波特率
  * @retval None
  ***********************************************************************************************************/
void Usart0_Init(int baudrate)
{
	USART_InitTypeDef USART_InitStructure;
	/* Enable peripheral clock of AFIO                                                              */
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
	CKCUClock.Bit.AFIO   = 1;
	CKCUClock.Bit.USART0  = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);

	/* Config AFIO mode as USART0_Rx and USART0_Tx function.
	*/
	AFIO_GPxConfig(GPIO_PA, AFIO_PIN_2, AFIO_FUN_USART_UART);
	AFIO_GPxConfig(GPIO_PA, AFIO_PIN_3, AFIO_FUN_USART_UART);
	
	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WORDLENGTH_8B;
	USART_InitStructure.USART_StopBits = USART_STOPBITS_1;
	USART_InitStructure.USART_Parity = USART_PARITY_NO;
	USART_InitStructure.USART_Mode = USART_MODE_NORMAL;
	USART_Init(HT_USART0, &USART_InitStructure);
	
//    rt_ringbuffer_init(&usart0_recv_ring_buf,USART0_RxBuf,USART0_RX_BUF_SIZE);                                                                             
	
	
	USART_TxCmd(HT_USART0, ENABLE);
	USART_RxCmd(HT_USART0, ENABLE);  
   
	USART_IntConfig(HT_USART0, USART_INT_RXDR, ENABLE);
                                                                   
	NVIC_EnableIRQ(USART0_IRQn);
}


#define USART_PDMA_IDLE 0
#define USART_PDMA_BUSY 1
PDMACH_InitTypeDef PDMACH_TxStructure;
PDMACH_InitTypeDef PDMACH_RxStructure;
vu8 USART_PDMA_TxState = USART_PDMA_IDLE;


void Usart0_IDELInit(int baudrate)
{
	USART_InitTypeDef USART_InitStructure;
	
	 /* Enable peripheral clock of AFIO, UxART, PDMA                                                           */
  CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
  CKCUClock.Bit.AFIO   = 1;
  CKCUClock.Bit.USART0  = 1;
  CKCUClock.Bit.PDMA   = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);

  /* Config AFIO mode as USART0_Rx and USART0_Tx function.                                                  */
  AFIO_GPxConfig(GPIO_PA, AFIO_PIN_2, AFIO_FUN_USART_UART);
	AFIO_GPxConfig(GPIO_PA, AFIO_PIN_3, AFIO_FUN_USART_UART);

  /* COM1 configuration ------------------------------------------------------------------------------------*/
	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WORDLENGTH_8B;
	USART_InitStructure.USART_StopBits = USART_STOPBITS_1;
	USART_InitStructure.USART_Parity = USART_PARITY_NO;
	USART_InitStructure.USART_Mode = USART_MODE_NORMAL;
	USART_Init(HT_USART0, &USART_InitStructure);

  USART_TxCmd(HT_USART0, ENABLE);
  USART_RxCmd(HT_USART0, ENABLE);

	USART_IntConfig(HT_USART0, USART_INT_RXDR, ENABLE); //开USART0接收中断                                                              
	NVIC_EnableIRQ(USART0_IRQn);

  /* COM1 Tx PDMA channel configuration        */
  PDMACH_TxStructure.PDMACH_SrcAddr = (u32)NULL;
  PDMACH_TxStructure.PDMACH_DstAddr = (u32)&HT_USART0->DR;
  PDMACH_TxStructure.PDMACH_BlkCnt = TBUFF_UNIT;
  PDMACH_TxStructure.PDMACH_BlkLen = 1;
  PDMACH_TxStructure.PDMACH_DataSize = WIDTH_8BIT;
  PDMACH_TxStructure.PDMACH_Priority = M_PRIO;
  PDMACH_TxStructure.PDMACH_AdrMod = SRC_ADR_LIN_INC | DST_ADR_FIX;
	PDMA_Config(PDMA_CH1, &PDMACH_TxStructure);
	
  PDMA_IntConfig(PDMA_CH1, PDMA_INT_GE | PDMA_INT_TC, ENABLE); //开启PDMA-USART0发送完成中断
  NVIC_EnableIRQ(PDMACH0_1_IRQn);

  USART_PDMACmd(HT_USART0, USART_PDMAREQ_TX, ENABLE);
	PDMA_EnaCmd(PDMA_CH1, DISABLE); //使能PDMA发送，等需要发送数据时才使能

  /* COM1 Rx PDMA channel configuration      */
//  PDMACH_RxStructure.PDMACH_SrcAddr = (u32)&HT_USART0->DR;
//  PDMACH_RxStructure.PDMACH_DstAddr = (u32)Usart0_RxBuff;
//  PDMACH_RxStructure.PDMACH_BlkCnt = USART0_RXBUFF_SIZE;
//  PDMACH_RxStructure.PDMACH_BlkLen = 1;
//  PDMACH_RxStructure.PDMACH_DataSize = WIDTH_8BIT;
//  PDMACH_RxStructure.PDMACH_Priority = H_PRIO;
//  PDMACH_RxStructure.PDMACH_AdrMod = SRC_ADR_FIX | DST_ADR_LIN_INC;
//  PDMA_Config(PDMA_CH0, &PDMACH_RxStructure);
//  
//	PDMA_EnaCmd(PDMA_CH0, ENABLE);
//  USART_PDMACmd(HT_USART0, USART_PDMAREQ_RX, ENABLE);
//不开启PDMA-USART0接收完成中断，而是在USART0接收处理函数Usart0_RecProcess里判断是否接收完成数据包
//  PDMA_IntConfig(PDMA_CH0, PDMA_INT_GE | PDMA_INT_TC, ENABLE); 

}

void PDMA_CH0_1_IRQHandler(void)
{
	if(PDMA_GetFlagStatus(PDMA_CH1,PDMA_FLAG_GE|PDMA_FLAG_TE)) //PDMA发送完成中断
  {
    /* Clear interrupt flags */    
    PDMA_ClearFlag(PDMA_CH1, PDMA_FLAG_GE|PDMA_FLAG_TE);
    USART_PDMACmd(HT_USART0,USART_PDMAREQ_TX,DISABLE);   
		DMA_flag = 0; 
  }
}

/********************************************************************************************************
**	函数名: 	void Usart0_SendSring(char* str)
**------------------------------------------------------------------------------------------------------- 
**	功能: 	向串口发送一串字符
**	形参 :  str字符串
**	返回值: 无
*******************************************************************************************************/
void Usart0_SendSring(char* str)
{
	while(*str!='\0')
	{
		USART_SendData(HT_USART0, *str);
		
		while (!USART_GetFlagStatus(HT_USART0, USART_FLAG_TXC));//此处不能使用sizeof
		str++;
	}
  
}

/*-------------------------------------------------*/
/*函数名：串口0 printf函数                         */
/*参  数：char* fmt,...  格式化输出字符串和参数    */
/*返回值：无                                       */
/*-------------------------------------------------*/

__align(8) char USART0_TxBuff[USART0_TXBUFF_SIZE];  

void u0_printf(char* fmt,...) 
{  
//	unsigned int i,length;
	
	va_list ap;
	va_start(ap,fmt);
	vsprintf(USART0_TxBuff,fmt,ap);
	va_end(ap);	
	
//	length=strlen((const char*)USART0_TxBuff);		
//	while((USART0->SR&0X40)==0);
//	for(i = 0;i < length;i ++)
//	{			
//		USART2->DR = USART2_TxBuff[i];
//		while((USART2->SR&0X40)==0);	
//	}	
	Usart0_SendSring(USART0_TxBuff);
}

void u0_TxData(unsigned char *data)
{
	DMA_flag = 1;     //DMA标志=1  要开始发送了
	PDMACH_TxStructure.PDMACH_SrcAddr = (u32)(&data[2]);
  PDMACH_TxStructure.PDMACH_BlkCnt = data[0]*256+data[1];
	PDMA_Config(PDMA_CH1, &PDMACH_TxStructure);
  PDMA_EnaCmd(PDMA_CH1, ENABLE);
  USART_PDMACmd(HT_USART0, USART_PDMAREQ_TX, ENABLE);
	
}


void USART0_IRQHandler(void)
{
 uint8_t receive_char;
	if (USART_GetFlagStatus(HT_USART0, USART_FLAG_RXDR)==SET)
  {
		Usart0RecTimer = 10;
		receive_char= USART_ReceiveData(HT_USART0);
		Usart0_RxBuff[Usart0_RxCounter++]=receive_char;
		USART_ClearFlag(HT_USART0,USART_FLAG_RXDR);
    return ;
  }
}



unsigned char Usart0_RecProcess(void)
{
	
	if(Usart0RecTimer) return FALSE;
	if(!Usart0_RxCounter) return FALSE;

//     sys_msg.msg_type = MSG_USART0_REC_FINISH;
//	 sys_msg.msg_parameter = Usart0_RxCounter;
//     Usart0_RxCounter = 0 ;//数据帧长度复位
//     rt_ringbuffer_put(&msg_ring_buf,(uint8_t *)&sys_msg.msg_type,2);//发送 串口1接收完成一帧消息(1字节) + 串口1接收帧的长度消息(1字节)
	
//收到数据包并处理
  
	if(Connect_flag==1)
	{
	memcpy(&MQTT_RxDataInPtr[2],Usart0_RxBuff,Usart0_RxCounter);                      //拷贝数据到接收缓冲区
	MQTT_RxDataInPtr[0] = Usart0_RxCounter/256;                                       //记录数据长度高字节
	MQTT_RxDataInPtr[1] = Usart0_RxCounter%256;                                       //记录数据长度低字节
	MQTT_RxDataInPtr+=RBUFF_UNIT;                                                     //指针下移
	if(MQTT_RxDataInPtr==MQTT_RxDataEndPtr)                                           //如果指针到缓冲区尾部了
			MQTT_RxDataInPtr = MQTT_RxDataBuf[0];                                         //指针归位到缓冲区开头
	
 }	
		
Usart0_RxCounter = 0;                                                       //串口2接收数据量变量清零	
	return TRUE;
}



