#include "config.h"
#include "bsp_uart1.h"
//extern unsigned char  HIMresponse[];//���ջ����� ����Ϊ��̬��������ֹ��ջ���
//extern unsigned int   HIMresponse_len;
struct rt_ringbuffer uart1_recv_ring_buf;//����1���ջ��λ������
uint8_t     s_UART1_RxBuf[UART1_RX_BUF_SIZE];//���ڽ��ջ�����
uint8_t 	s_UART1_RecFrameLen = 0;//��ŵ�ǰ���ڽ������ݴ�ŵ�λ��
/*********************************************************************************************************//**
  * @brief UART1_int  uart1��ʼ��
  * @param  baudrate :������
  * @retval None
  ***********************************************************************************************************/
void Uart1_Init(int baudrate)
{

	USART_InitTypeDef USART_InitStructure;
  /* Enable peripheral clock of AFIO                                                              */
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
	CKCUClock.Bit.AFIO   = 1;
	CKCUClock.Bit.UART1  = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);

  /* Config AFIO mode as USART0_Rx and USART0_Tx function.*/
	
	AFIO_GPxConfig(GPIO_PB, AFIO_PIN_4, AFIO_FUN_USART_UART);
	AFIO_GPxConfig(GPIO_PB, AFIO_PIN_5, AFIO_FUN_USART_UART);

	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WORDLENGTH_8B;
	USART_InitStructure.USART_StopBits = USART_STOPBITS_1;
	USART_InitStructure.USART_Parity = USART_PARITY_NO;
	USART_InitStructure.USART_Mode = USART_MODE_NORMAL;

	USART_Init(HT_UART1, &USART_InitStructure);
    
	rt_ringbuffer_init(&uart1_recv_ring_buf,s_UART1_RxBuf,UART1_RX_BUF_SIZE);	
	
	USART_TxCmd(HT_UART1, ENABLE);
	USART_RxCmd(HT_UART1, ENABLE);
	USART_IntConfig(HT_UART1, USART_INT_RXDR, ENABLE);
	/* Configure USART0 & USART1 interrupt                                                                    */
	NVIC_EnableIRQ(UART1_IRQn);
}
/********************************************************************************************************
**	������: 	void Uart1_SendSring(char* str)
**------------------------------------------------------------------------------------------------------- 
**	����: 	�򴮿ڷ���һ���ַ�
**	�β� :  str�ַ���
**	����ֵ: ��
*******************************************************************************************************/
void Uart1_SendSring(char* str)
{
	while(*str!='\0')
	{
		USART_SendData(HT_UART1, *str);		
		while (!USART_GetFlagStatus(HT_UART1, USART_FLAG_TXC));//�˴�����ʹ��sizeof
		str++;
	}
  
}
int fputc(int ch,FILE *f)
{
	USART_SendData(HT_UART1,ch);
	while(USART_GetFlagStatus(HT_UART1,USART_FLAG_TXC)==RESET);//�ȴ��������
	return (ch);	
}
void UART1_IRQHandler(void)
{
  uint8_t receive_char;	
	if(USART_GetFlagStatus(HT_UART1,USART_FLAG_RXDR)) //���շǿ��ж�
    {	
		Uart1RecTimer = 10;
		receive_char= USART_ReceiveData(HT_UART1);
		rt_ringbuffer_put(&uart1_recv_ring_buf,&receive_char,1);
		s_UART1_RecFrameLen++;
		USART_ClearFlag(HT_UART1,USART_FLAG_RXDR);
	} 
}
static system_message  UART1sys_msg;
unsigned char Uart1_RecProcess(void)
{
	if(Uart1RecTimer) return FALSE;
	if(!s_UART1_RecFrameLen) return FALSE;
			UART1sys_msg.msg_type = MSG_UART1_REC_FINISH;
			UART1sys_msg.msg_parameter = s_UART1_RecFrameLen;
			s_UART1_RecFrameLen = 0 ;//����֡���ȸ�λ
			rt_ringbuffer_put(&msg_ring_buf,(uint8_t *)&UART1sys_msg.msg_type,2);//���� ����1�������һ֡��Ϣ(1�ֽ�) + ����1����֡�ĳ�����Ϣ(1�ֽ�)
	
	return TRUE;
}
