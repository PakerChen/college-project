#include "bsp_BFTM.h"

//extern int compare;
void BF_TIM_32s_int(void) 
{

	
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
  CKCUClock.Bit.BFTM0      = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);

	CKCU_SetPeripPrescaler(CKCU_PCLK_BFTM0, CKCU_APBCLKPRE_DIV32);
	
	
  NVIC_EnableIRQ(BFTM0_IRQn);

  BFTM_SetCompare(HT_BFTM0, SystemCoreClock);//32��һ��
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

  BFTM_SetCompare(HT_BFTM0, SystemCoreClock/4);//2��һ��
  BFTM_SetCounter(HT_BFTM0, 0);
  BFTM_IntConfig(HT_BFTM0, ENABLE);
  BFTM_EnaCmd(HT_BFTM0, ENABLE);
}

/*********************************************************************************************************//**
 * @brief   This function handles BFTM interrupt.��ʱ���ж�
 * @retval  None
 ************************************************************************************************************/
void BFTM0_IRQHandler(void)
{ 

  if(BFTM_GetFlagStatus(HT_BFTM0)==SET)
//	  printf("Hello!\r\n");
	switch(Ping_flag){                               //�ж�Ping_flag��״̬
			case 0:										 //���Ping_flag����0����ʾ����״̬������Ping����  
					MQTT_PingREQ(); 					 //���Ping���ĵ����ͻ�����  
					break;
			case 1:										 //���Ping_flag����1��˵����һ�η��͵���ping���ģ�û���յ��������ظ�������1û�б����Ϊ0�������������쳣������Ҫ��������pingģʽ
					BF_TIM_2s_int(); 					 //���ǽ���ʱ��6����Ϊ2s��ʱ,���ٷ���Ping����
					MQTT_PingREQ();  					 //���Ping���ĵ����ͻ�����  
					break;
			case 2:										 //���Ping_flag����2��˵����û���յ��������ظ�
			case 3:				                         //���Ping_flag����3��˵����û���յ��������ظ�
			case 4:				                         //���Ping_flag����4��˵����û���յ��������ظ�	
					MQTT_PingREQ();  					 //���Ping���ĵ����ͻ����� 
					break;
			case 5:										 //���Ping_flag����5��˵�����Ƿ����˶��ping�����޻ظ���Ӧ�������������⣬������������
					Connect_flag = 0;                    //����״̬��0����ʾ�Ͽ���û���Ϸ�����
					  BFTM_EnaCmd(HT_BFTM0, DISABLE);              //�ر�BFTM0 				
					break;			
		}
		Ping_flag++;           		             		 //Ping_flag����1����ʾ�ַ�����һ��ping���ڴ��������Ļظ�
	
	
	
  BFTM_ClearFlag(HT_BFTM0);
} 

