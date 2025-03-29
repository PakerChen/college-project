#include "bsp_exti.h"


void EXTIX_Init(void)
{ 
	EXTI_InitTypeDef EXTI_InitStruct = {0};
	
  CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
	CKCUClock.Bit.PA = 1;                            
  CKCUClock.Bit.EXTI       = 1;
	CKCUClock.Bit.AFIO  =1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);

    /* Connect Button EXTI Channel to Button GPIO Pin                                                       */
  AFIO_EXTISourceConfig(AFIO_EXTI_CH_8,(AFIO_ESS_Enum) GPIO_PA);


  EXTI_InitStruct.EXTI_Channel = EXTI_CHANNEL_8;
  EXTI_InitStruct.EXTI_Debounce = EXTI_DEBOUNCE_DISABLE; //������ʧ��
  EXTI_InitStruct.EXTI_DebounceCnt = 0;
  EXTI_InitStruct.EXTI_IntType =  EXTI_POSITIVE_EDGE ; //�����ش���
  EXTI_Init(&EXTI_InitStruct);
		
		
		  /* Enable Button EXTI Interrupt                                                                         */
  EXTI_IntConfig(EXTI_CHANNEL_8, ENABLE);
   
  NVIC_EnableIRQ(EXTI4_15_IRQn);
   
}
//u8 ov_sta;	//֡�жϱ��
 //�ⲿ�ж�4~15�������

void EXTI4_15_IRQHandler(void)
{

 if(EXTI_GetEdgeStatus(EXTI_CHANNEL_8,EXTI_EDGE_POSITIVE))
  {
    //�û�����
		
		
		
    EXTI_ClearEdgeFlag(EXTI_CHANNEL_8);
  }
	
		
}









