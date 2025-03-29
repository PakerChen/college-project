#ifndef _BSP_PWM_H
#define _BSP_PWM_H

#include "config.h"

#if defined(USE_HT32F52352_SK)
 

  #define HTCFG_OUTPUT_PRESCALER                          12000
  #define HTCFG_OUTPUT_PWM_BY_SCTM_GPIO_ID                (GPIO_PD)
  #define HTCFG_OUTPUT_PWM_BY_SCTM_AFIO_PIN               (AFIO_PIN_0)
  #define HTCFG_OUTPUT_PWM_BY_SCTM                        (HT_SCTM0)
  #define HTCFG_OUTPUT_PWM_BY_AFIO_MODE                   (AFIO_FUN_SCTM)
  #define HTCFG_OUTPUT_PWM_BY_SCTM_CHANNEL                (TM_CH_0)
  #define HTCFG_OUTPUT_PWM_BY_SCTM_CLK(CK)                (CK.Bit.SCTM0)
#endif


void SCTM_Configuration(void);     

#endif


