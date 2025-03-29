#include "config.h"
#include "UART0.h"


struct rt_ringbuffer uart0_recv_ring_buf;//串口1接收环形缓冲队列
uint8_t     s_UART0_RxBuf[UART0_RX_BUF_SIZE];//串口接收缓冲区
uint8_t 	s_UART0_RecFrameLen = 0;//存放当前串口接收数据存放的位置
/*********************************************************************************************************//**
  * @brief UART1_int  uart1初始化
  * @param  baudrate :波特率
  * @retval None
  ***********************************************************************************************************/
void Uart0_Init(int baudrate)
{

	USART_InitTypeDef USART_InitStructure;
  /* Enable peripheral clock of AFIO                                                              */
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
	CKCUClock.Bit.AFIO   = 1;
	CKCUClock.Bit.UART0  = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);

  /* Config AFIO mode as USART0_Rx and USART0_Tx function.*/
	
	AFIO_GPxConfig(GPIO_PC, AFIO_PIN_4, AFIO_FUN_USART_UART);
	AFIO_GPxConfig(GPIO_PC, AFIO_PIN_5, AFIO_FUN_USART_UART);

	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WORDLENGTH_8B;
	USART_InitStructure.USART_StopBits = USART_STOPBITS_1;
	USART_InitStructure.USART_Parity = USART_PARITY_NO;
	USART_InitStructure.USART_Mode = USART_MODE_NORMAL;

	USART_Init(HT_UART0, &USART_InitStructure);
    
	rt_ringbuffer_init(&uart0_recv_ring_buf,s_UART0_RxBuf,UART0_RX_BUF_SIZE);	
	
	USART_TxCmd(HT_UART0, ENABLE);
	USART_RxCmd(HT_UART0, ENABLE);
	USART_IntConfig(HT_UART0, USART_INT_RXDR, ENABLE);
	/* Configure USART0 & USART1 interrupt                                                                    */
	NVIC_EnableIRQ(UART0_IRQn);
}
void Uart0_SendSring(char* str)
{
	while(*str!='\0')
	{
		USART_SendData(HT_UART0, *str);
		
		while (!USART_GetFlagStatus(HT_UART0, USART_FLAG_TXC));//此处不能使用sizeof
		str++;
	}
  
}


__align(8) char UART0_TxBuff[UART0_TXBUFF_SIZE];  

void UART0_printf(char* fmt,...) 
{  
	va_list ap;
	va_start(ap,fmt);
	vsprintf(UART0_TxBuff,fmt,ap);
	va_end(ap);	
	Uart0_SendSring(UART0_TxBuff);
}

void UART0_IRQHandler(void)
{
  uint8_t receive_char;
	
	if(USART_GetFlagStatus(HT_UART0,USART_FLAG_RXDR)) //接收非空中断
    {
		Uart0RecTimer = 10;
		receive_char= USART_ReceiveData(HT_UART0);
		rt_ringbuffer_put(&uart0_recv_ring_buf,&receive_char,1);
		s_UART0_RecFrameLen++;
		USART_ClearFlag(HT_UART0,USART_FLAG_RXDR);
	}
	  
}

static system_message  UART0SYS_msg;
unsigned char Uart0_RecProcess(void)
{
	
	if(Uart0RecTimer) return FALSE;
	if(!s_UART0_RecFrameLen) return FALSE;

     UART0SYS_msg.msg_type = MSG_UART0_REC_FINISH;
			UART0SYS_msg.msg_parameter = s_UART0_RecFrameLen;
			s_UART0_RecFrameLen = 0 ;//数据帧长度复位
			rt_ringbuffer_put(&msg_ring_buf,(uint8_t *)&UART0SYS_msg.msg_type,2);//发送 串口1接收完成一帧消息(1字节) + 串口1接收帧的长度消息(1字节)
			

	return TRUE;
}

