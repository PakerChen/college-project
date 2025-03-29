#include "bsp_pwm.h"


void SCTM_Configuration(void)
{
  TM_TimeBaseInitTypeDef TM_TimeBaseInitStructure;
  TM_OutputInitTypeDef TM_OutputInitStructure;
	
  CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
  CKCUClock.Bit.AFIO                            = 1; 

  HTCFG_OUTPUT_PWM_BY_SCTM_CLK(CKCUClock)       = 1;//	CKCUClock.Bit.SCTM0 =1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);

  AFIO_GPxConfig(HTCFG_OUTPUT_PWM_BY_SCTM_GPIO_ID, HTCFG_OUTPUT_PWM_BY_SCTM_AFIO_PIN, HTCFG_OUTPUT_PWM_BY_AFIO_MODE) ;
  
	/* Init SCTM0 time-base for PWM  频率1Hz                                                                         */
  TM_TimeBaseInitStructure.Prescaler = HTCFG_OUTPUT_PRESCALER-1; //psc
  TM_TimeBaseInitStructure.CounterReload = SystemCoreClock/(TM_TimeBaseInitStructure.Prescaler + 1)-1;    //arr
  TM_TimeBaseInitStructure.RepetitionCounter = 0;
  TM_TimeBaseInitStructure.CounterMode = TM_CNT_MODE_UP;
  TM_TimeBaseInitStructure.PSCReloadTime = TM_PSC_RLD_IMMEDIATE;
  TM_TimeBaseInit(HTCFG_OUTPUT_PWM_BY_SCTM, &TM_TimeBaseInitStructure);

  /* Clear Update Event Interrupt flag                                                                      */
  TM_ClearFlag(HTCFG_OUTPUT_PWM_BY_SCTM, TM_FLAG_UEV);

  TM_OutputInitStructure.Channel = HTCFG_OUTPUT_PWM_BY_SCTM_CHANNEL;
  TM_OutputInitStructure.OutputMode = TM_OM_PWM2;
  TM_OutputInitStructure.Control = TM_CHCTL_ENABLE;
  TM_OutputInitStructure.ControlN = TM_CHCTL_DISABLE;//TM_CHCTL_ENABLE;
  TM_OutputInitStructure.Polarity = TM_CHP_NONINVERTED;
  TM_OutputInitStructure.PolarityN = TM_CHP_NONINVERTED;
  TM_OutputInitStructure.IdleState = MCTM_OIS_LOW;
  TM_OutputInitStructure.IdleStateN = MCTM_OIS_HIGH;
//  TM_OutputInitStructure.Compare = SystemCoreClock/(TM_TimeBaseInitStructure.Prescaler + 1)/2; //占空比50%
	TM_OutputInitStructure.Compare = SystemCoreClock/(TM_TimeBaseInitStructure.Prescaler + 1)/4; //占空比75%
//		TM_OutputInitStructure.Compare = (SystemCoreClock/(TM_TimeBaseInitStructure.Prescaler + 1))-10; 	
		
		
  TM_OutputInit(HTCFG_OUTPUT_PWM_BY_SCTM, &TM_OutputInitStructure);

  /* SCTM0/PWM counter enable                                                                               */
  TM_Cmd(HTCFG_OUTPUT_PWM_BY_SCTM, ENABLE);
}


