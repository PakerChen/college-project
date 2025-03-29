#include "bsp_BFTM.h"

//extern int compare;
void BF_TIM_32s_int(void) 
{

	
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
  CKCUClock.Bit.BFTM0      = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);

	CKCU_SetPeripPrescaler(CKCU_PCLK_BFTM0, CKCU_APBCLKPRE_DIV32);
	
	
  NVIC_EnableIRQ(BFTM0_IRQn);

  BFTM_SetCompare(HT_BFTM0, SystemCoreClock);//32秒一次
  BFTM_SetCounter(HT_BFTM0, 0);
  BFTM_IntConfig(HT_BFTM0, ENABLE);
  BFTM_EnaCmd(HT_BFTM0, ENABLE);
}

void BF_TIM_2s_int(void) 
{

	
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
  CKCUClock.Bit.BFTM0      = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);

	CKCU_SetPeripPrescaler(CKCU_PCLK_BFTM0, CKCU_APBCLKPRE_DIV8);
	
  NVIC_EnableIRQ(BFTM0_IRQn);

  BFTM_SetCompare(HT_BFTM0, SystemCoreClock/4);//2秒一次
  BFTM_SetCounter(HT_BFTM0, 0);
  BFTM_IntConfig(HT_BFTM0, ENABLE);
  BFTM_EnaCmd(HT_BFTM0, ENABLE);
}

/*********************************************************************************************************//**
 * @brief   This function handles BFTM interrupt.定时器中断
 * @retval  None
 ************************************************************************************************************/
void BFTM0_IRQHandler(void)
{ 

  if(BFTM_GetFlagStatus(HT_BFTM0)==SET)
//	  printf("Hello!\r\n");
	switch(Ping_flag){                               //判断Ping_flag的状态
			case 0:										 //如果Ping_flag等于0，表示正常状态，发送Ping报文  
					MQTT_PingREQ(); 					 //添加Ping报文到发送缓冲区  
					break;
			case 1:										 //如果Ping_flag等于1，说明上一次发送到的ping报文，没有收到服务器回复，所以1没有被清除为0，可能是连接异常，我们要启动快速ping模式
					BF_TIM_2s_int(); 					 //我们将定时器6设置为2s定时,快速发送Ping报文
					MQTT_PingREQ();  					 //添加Ping报文到发送缓冲区  
					break;
			case 2:										 //如果Ping_flag等于2，说明还没有收到服务器回复
			case 3:				                         //如果Ping_flag等于3，说明还没有收到服务器回复
			case 4:				                         //如果Ping_flag等于4，说明还没有收到服务器回复	
					MQTT_PingREQ();  					 //添加Ping报文到发送缓冲区 
					break;
			case 5:										 //如果Ping_flag等于5，说明我们发送了多次ping，均无回复，应该是连接有问题，我们重启连接
					Connect_flag = 0;                    //连接状态置0，表示断开，没连上服务器
					  BFTM_EnaCmd(HT_BFTM0, DISABLE);              //关闭BFTM0 				
					break;			
		}
		Ping_flag++;           		             		 //Ping_flag自增1，表示又发送了一次ping，期待服务器的回复
	
	
	
  BFTM_ClearFlag(HT_BFTM0);
} 

