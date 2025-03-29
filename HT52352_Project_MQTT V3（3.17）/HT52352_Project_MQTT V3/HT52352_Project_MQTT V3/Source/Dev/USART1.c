#include "config.h"
#include "USART1.h"


struct rt_ringbuffer usart1_recv_ring_buf;//串口1接收环形缓冲队列
uint8_t     s_USART1_RxBuf[USART1_RX_BUF_SIZE];//串口接收缓冲区
uint8_t 	s_USART1_RecFrameLen = 0;//存放当前串口接收数据存放的位置
/*********************************************************************************************************//**
  * @brief UART1_int  uart1初始化
  * @param  baudrate :波特率
  * @retval None
  ***********************************************************************************************************/
void Usart1_Init(int baudrate)
{

	USART_InitTypeDef USART_InitStructure;
  /* Enable peripheral clock of AFIO                                                              */
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
	CKCUClock.Bit.AFIO   = 1;
	CKCUClock.Bit.USART1  = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);

  /* Config AFIO mode as USART0_Rx and USART0_Tx function.*/
	
	AFIO_GPxConfig(GPIO_PA, AFIO_PIN_4, AFIO_FUN_USART_UART);
	AFIO_GPxConfig(GPIO_PA, AFIO_PIN_5, AFIO_FUN_USART_UART);

	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WORDLENGTH_8B;
	USART_InitStructure.USART_StopBits = USART_STOPBITS_1;
	USART_InitStructure.USART_Parity = USART_PARITY_NO;
	USART_InitStructure.USART_Mode = USART_MODE_NORMAL;

	USART_Init(HT_USART1, &USART_InitStructure);
    
	rt_ringbuffer_init(&usart1_recv_ring_buf,s_USART1_RxBuf,USART1_RX_BUF_SIZE);	
	
	USART_TxCmd(HT_USART1, ENABLE);
	USART_RxCmd(HT_USART1, ENABLE);
	USART_IntConfig(HT_USART1, USART_INT_RXDR, ENABLE);
	/* Configure USART0 & USART1 interrupt                                                                    */
	NVIC_EnableIRQ(USART1_IRQn);
}
void Usart1_SendSring(char* str)
{
	while(*str!='\0')
	{
		USART_SendData(HT_USART1, *str);
		while (!USART_GetFlagStatus(HT_USART1, USART_FLAG_TXC));//此处不能使用sizeof
		str++;
	}
}
__align(8) char USART1_TxBuff[USART1_TXBUFF_SIZE];  

void USART1_printf(char* fmt,...) 
{  
	va_list ap;
	va_start(ap,fmt);
	vsprintf(USART1_TxBuff,fmt,ap);
	va_end(ap);	
	Usart1_SendSring(USART1_TxBuff);
}
void USART1_IRQHandler(void)
{
  uint8_t receive_char;
	
	if(USART_GetFlagStatus(HT_USART1,USART_FLAG_RXDR)) //接收非空中断
    {
		Usart1RecTimer = 10;
		receive_char= USART_ReceiveData(HT_USART1);
		rt_ringbuffer_put(&usart1_recv_ring_buf,&receive_char,1);
		s_USART1_RecFrameLen++;
		USART_ClearFlag(HT_USART1,USART_FLAG_RXDR);
	}
	  
}
static system_message  USART1SYS_msg;
unsigned char Usart1_RecProcess(void)
{
	
	if(Usart1RecTimer) return FALSE;
	if(!s_USART1_RecFrameLen) return FALSE;

     USART1SYS_msg.msg_type = MSG_USART1_REC_FINISH;
			USART1SYS_msg.msg_parameter = s_USART1_RecFrameLen;
			s_USART1_RecFrameLen = 0 ;//数据帧长度复位
			rt_ringbuffer_put(&msg_ring_buf,(uint8_t *)&USART1SYS_msg.msg_type,2);//发送 串口1接收完成一帧消息(1字节) + 串口1接收帧的长度消息(1字节)
			

	return TRUE;
}
