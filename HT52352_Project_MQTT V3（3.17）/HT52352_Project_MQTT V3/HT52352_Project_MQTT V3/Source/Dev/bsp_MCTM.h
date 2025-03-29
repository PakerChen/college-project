#ifndef _BSP_MCTM_H
#define _BSP_MCTM_H

#include "config.h"


/* Settings ---------------------------------------------------------------*/

#if defined(USE_HT32F52352_SK)
  #define HTCFG_MCTM_CH0_GPIO_ID                      (GPIO_PB)
  #define HTCFG_MCTM_CH0N_GPIO_ID                     (GPIO_PC)
  #define HTCFG_MCTM_CH1_GPIO_ID                      (GPIO_PB)
  #define HTCFG_MCTM_CH1N_GPIO_ID                     (GPIO_PB)
  #define HTCFG_MCTM_CH2_GPIO_ID                      (GPIO_PD)
  #define HTCFG_MCTM_CH2N_GPIO_ID                     (GPIO_PD)
  #define HTCFG_MCTM_BREAK_GPIO_ID                    (GPIO_PB)

  #define HTCFG_MCTM_CH0_AFIO_PIN                     (AFIO_PIN_15)
  #define HTCFG_MCTM_CH0N_AFIO_PIN                    (AFIO_PIN_0)
  #define HTCFG_MCTM_CH1_AFIO_PIN                     (AFIO_PIN_0)
  #define HTCFG_MCTM_CH1N_AFIO_PIN                    (AFIO_PIN_1)
  #define HTCFG_MCTM_CH2_AFIO_PIN                     (AFIO_PIN_1)
  #define HTCFG_MCTM_CH2N_AFIO_PIN                    (AFIO_PIN_2)
  #define HTCFG_MCTM_BREAK_AFIO_PIN                   (AFIO_PIN_4)
  #define HTCFG_MCTM_RELOAD                           (48000000/2000)
  #define HTCFG_MCTM_DEAD_TIME                        (72)
#endif



void MCTM_Configuration(void);     

#endif
