#ifndef _BSP_GPTM_H
#define _BSP_GPTM_H

#include "config.h"

#if defined(USE_HT32F52352_SK)
  #define HTCFG_INPUT_CAPTURE_GPTM_GPIO_ID                (GPIO_PC)
  #define HTCFG_INPUT_CAPTURE_GPTM_AFIO_PIN               (AFIO_PIN_5)
  #define HTCFG_INPUT_CAPTURE_GPTM_CHANNEL                (TM_CH_1)
  #define HTCFG_INPUT_CAPTURE_GPTM_IRQn                   (GPTM0_IRQn)
  #define HTCFG_INPUT_CAPTURE_GPTM                        (HT_GPTM0)
  #define HTCFG_INPUT_CAPTURE_GPTM_AFIO_MODE              (AFIO_FUN_MCTM_GPTM)
  #define HTCFG_INPUT_CAPTURE_GPTM_CLK(CK)                (CK.Bit.GPTM0)
  #define HTCFG_INPUT_CAPTURE_GPTM_CCR                    (TM_INT_CH1CC)



#endif


/* Private types ----------------------------------------------------------------------*/
typedef struct
{
  u32 OverflowCounter;
  u32 StartValue;
  u32 CapturePulse;
  TM_CHP_Enum ChannelPolarity;
  bool DataValid;
  bool DataOverwrite;
} sPulseCaptureStructure;

extern sPulseCaptureStructure CaptureCHx;


void GTCM_Configuration(void);     
void CaptureProcess(sPulseCaptureStructure* cap, u16 capture_value, bool isCapBeforeUpdate);



#endif


