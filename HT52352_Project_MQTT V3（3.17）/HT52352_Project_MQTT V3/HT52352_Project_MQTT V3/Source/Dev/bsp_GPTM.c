#include "bsp_GPTM.h"

sPulseCaptureStructure CaptureCHx;


void GTCM_Configuration(void)
{
  CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
  
  /* Enable AFIO, GPIO and GPTM PCLK                                                                        */
  CKCUClock.Bit.AFIO       = 1;
  HTCFG_INPUT_CAPTURE_GPTM_CLK(CKCUClock)      = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);
	
	AFIO_GPxConfig(HTCFG_INPUT_CAPTURE_GPTM_GPIO_ID, HTCFG_INPUT_CAPTURE_GPTM_AFIO_PIN, HTCFG_INPUT_CAPTURE_GPTM_AFIO_MODE) ;
	
	
	TM_CaptureInitTypeDef  CapInit;
  CapInit.Channel = HTCFG_INPUT_CAPTURE_GPTM_CHANNEL;
  CapInit.Polarity = TM_CHP_NONINVERTED;
  CapInit.Selection = TM_CHCCS_DIRECT;
  CapInit.Prescaler = TM_CHPSC_OFF;
  CapInit.Filter = 0x0;
  TM_CaptureInit(HTCFG_INPUT_CAPTURE_GPTM, &CapInit);
  /* Enable GPTM                                                                                            */
  TM_Cmd(HTCFG_INPUT_CAPTURE_GPTM, ENABLE);
  /* Enable GPTM Channel Capture interrupts and Update Event interrupt                                      */
  NVIC_EnableIRQ(HTCFG_INPUT_CAPTURE_GPTM_IRQn);
  TM_IntConfig(HTCFG_INPUT_CAPTURE_GPTM, HTCFG_INPUT_CAPTURE_GPTM_CCR | TM_INT_UEV, ENABLE);
}


/*********************************************************************************************************//**
 * @brief   CaptureProcess function.
 * @retval  None
 ************************************************************************************************************/
void CaptureProcess(sPulseCaptureStructure* cap, u16 capture_value, bool isCapBeforeUpdate)
{
  if (cap->ChannelPolarity == TM_CHP_NONINVERTED)  /*!< TM channel polarity is active high or rising edge  */
  {
    /* Reset OverflowCounter and store capture value when rising edge occurred                              */
    if (isCapBeforeUpdate)
    {
      cap->OverflowCounter = 1;
    }
    else
    {
      cap->OverflowCounter = 0;
    }

    cap->StartValue = capture_value;

    /* Change channel polarity to capture when falling edge occur                                           */
    cap->ChannelPolarity = TM_CHP_INVERTED;
  }
  else
  {
    /* Compute pulse width in PCLK unit when falling edge occurred                                          */
    if (isCapBeforeUpdate)
      cap->OverflowCounter--;

    cap->CapturePulse = (cap->OverflowCounter << 16) + capture_value - cap->StartValue;

    if (cap->DataValid)
      cap->DataOverwrite = TRUE;
    else
      cap->DataValid = TRUE;

    /* Change channel polarity to capture when rising edge occur                                            */
    cap->ChannelPolarity = TM_CHP_NONINVERTED;
  }
}

/*********************************************************************************************************//**
 * @brief   This function handles General Purpose Timer interrupt.
 * @retval  None
 ************************************************************************************************************/
void GPTM0_IRQHandler(void)
{
  bool update_flag = FALSE;

  /* store and clear all interrupt flags                                                                    */
  u32 status = HTCFG_INPUT_CAPTURE_GPTM->INTSR;
  u32 cnt = HTCFG_INPUT_CAPTURE_GPTM->CNTR;
  if ((status & TM_INT_UEV) != (HTCFG_INPUT_CAPTURE_GPTM->INTSR & TM_INT_UEV))
  {
    status = HTCFG_INPUT_CAPTURE_GPTM->INTSR;
    cnt = HTCFG_INPUT_CAPTURE_GPTM->CNTR;
  }
  HTCFG_INPUT_CAPTURE_GPTM->INTSR = ~status;

  if (status & TM_INT_UEV)
  {
    update_flag = TRUE;
    /* The OverflowCounter will stop at max value 0xFFFF                                                    */
    if (CaptureCHx.OverflowCounter != 0xFFFF) CaptureCHx.OverflowCounter++;
  }

  if (status & HTCFG_INPUT_CAPTURE_GPTM_CCR)
  {
    u32 cap_value = TM_GetCaptureCompare1(HTCFG_INPUT_CAPTURE_GPTM);
    bool isCapBeforeUpdate = (update_flag && (cap_value > cnt))? TRUE : FALSE;
    CaptureProcess(&CaptureCHx, cap_value, isCapBeforeUpdate);
    TM_ChPolarityConfig(HTCFG_INPUT_CAPTURE_GPTM, HTCFG_INPUT_CAPTURE_GPTM_CHANNEL, CaptureCHx.ChannelPolarity);
  }
}












