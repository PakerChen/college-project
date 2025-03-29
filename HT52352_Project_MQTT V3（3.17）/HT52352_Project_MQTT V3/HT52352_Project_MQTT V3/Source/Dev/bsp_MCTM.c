#include "bsp_MCTM.h"

void MCTM_Configuration(void)
{
  /* !!! NOTICE !!!
     Notice that the local variable (structure) did not have an initial value.
     Please confirm that there are no missing members in the parameter settings below in this function.
  */
  TM_TimeBaseInitTypeDef MCTM_TimeBaseInitStructure;
  TM_OutputInitTypeDef MCTM_OutputInitStructure;
  MCTM_CHBRKCTRInitTypeDef MCTM_CHBRKCTRInitStructure;

  CKCU_PeripClockConfig_TypeDef CKCUClock = {{ 0 }};
  CKCUClock.Bit.AFIO       = 1;
  CKCUClock.Bit.MCTM0      = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);

  /* Configure MCTM Channel 0 output pin                                                                    */
  AFIO_GPxConfig(HTCFG_MCTM_CH0_GPIO_ID, HTCFG_MCTM_CH0_AFIO_PIN, AFIO_FUN_MCTM_GPTM);
  AFIO_GPxConfig(HTCFG_MCTM_CH0N_GPIO_ID, HTCFG_MCTM_CH0N_AFIO_PIN, AFIO_FUN_MCTM_GPTM);
  /* Configure MCTM Channel 1 output pin                                                                    */
  AFIO_GPxConfig(HTCFG_MCTM_CH1_GPIO_ID, HTCFG_MCTM_CH1_AFIO_PIN, AFIO_FUN_MCTM_GPTM);
  AFIO_GPxConfig(HTCFG_MCTM_CH1N_GPIO_ID, HTCFG_MCTM_CH1N_AFIO_PIN, AFIO_FUN_MCTM_GPTM);
  /* Configure MCTM Channel 2 output pin                                                                    */
  AFIO_GPxConfig(HTCFG_MCTM_CH2_GPIO_ID, HTCFG_MCTM_CH2_AFIO_PIN, AFIO_FUN_MCTM_GPTM);
  AFIO_GPxConfig(HTCFG_MCTM_CH2N_GPIO_ID, HTCFG_MCTM_CH2N_AFIO_PIN, AFIO_FUN_MCTM_GPTM);
  /* Configure MCTM Break pin                                                                               */
  AFIO_GPxConfig(HTCFG_MCTM_BREAK_GPIO_ID, HTCFG_MCTM_BREAK_AFIO_PIN, AFIO_FUN_MCTM_GPTM);

  /* MCTM Time Base configuration                                                                           */
  MCTM_TimeBaseInitStructure.CounterReload = HTCFG_MCTM_RELOAD - 1;
  MCTM_TimeBaseInitStructure.Prescaler = 0;
  MCTM_TimeBaseInitStructure.RepetitionCounter = 0;
  MCTM_TimeBaseInitStructure.CounterMode = TM_CNT_MODE_UP;
  MCTM_TimeBaseInitStructure.PSCReloadTime = TM_PSC_RLD_IMMEDIATE;
  TM_TimeBaseInit(HT_MCTM0, &MCTM_TimeBaseInitStructure);

  /* MCTM Channel 0, 1, 2 output configuration                                                              */
  MCTM_OutputInitStructure.Channel = TM_CH_0;
  MCTM_OutputInitStructure.OutputMode = TM_OM_PWM1;
  MCTM_OutputInitStructure.Control = TM_CHCTL_ENABLE;
  MCTM_OutputInitStructure.ControlN = TM_CHCTL_ENABLE;
  MCTM_OutputInitStructure.Polarity = TM_CHP_NONINVERTED;
  MCTM_OutputInitStructure.PolarityN = TM_CHP_NONINVERTED;
  MCTM_OutputInitStructure.IdleState = MCTM_OIS_LOW;
  MCTM_OutputInitStructure.IdleStateN = MCTM_OIS_HIGH;
  MCTM_OutputInitStructure.Compare = HTCFG_MCTM_RELOAD * 3 / 4;
  MCTM_OutputInitStructure.AsymmetricCompare = 0;
  TM_OutputInit(HT_MCTM0, &MCTM_OutputInitStructure);

  MCTM_OutputInitStructure.Channel = TM_CH_1;
  MCTM_OutputInitStructure.Compare = HTCFG_MCTM_RELOAD * 1 / 2;
  TM_OutputInit(HT_MCTM0, &MCTM_OutputInitStructure);

  MCTM_OutputInitStructure.Channel = TM_CH_2;
  MCTM_OutputInitStructure.Compare = HTCFG_MCTM_RELOAD * 1 / 4;
  TM_OutputInit(HT_MCTM0, &MCTM_OutputInitStructure);

  /* MCTM Off State, lock, Break, Automatic Output enable, dead time configuration                          */
  MCTM_CHBRKCTRInitStructure.OSSRState = MCTM_OSSR_STATE_ENABLE;
  MCTM_CHBRKCTRInitStructure.OSSIState = MCTM_OSSI_STATE_ENABLE;
  MCTM_CHBRKCTRInitStructure.LockLevel = MCTM_LOCK_LEVEL_2;
  MCTM_CHBRKCTRInitStructure.Break0 = MCTM_BREAK_ENABLE;
  MCTM_CHBRKCTRInitStructure.Break0Polarity = MCTM_BREAK_POLARITY_LOW;
  MCTM_CHBRKCTRInitStructure.AutomaticOutput = MCTM_CHAOE_ENABLE;
  MCTM_CHBRKCTRInitStructure.DeadTime = HTCFG_MCTM_DEAD_TIME;
  MCTM_CHBRKCTRInitStructure.BreakFilter = 0;
  MCTM_CHBRKCTRConfig(HT_MCTM0, &MCTM_CHBRKCTRInitStructure);

  /* MCTM counter enable                                                                                    */
  TM_Cmd(HT_MCTM0, ENABLE);

  /* MCTM Channel Main Output enable                                                                        */
  MCTM_CHMOECmd(HT_MCTM0, ENABLE);
}


