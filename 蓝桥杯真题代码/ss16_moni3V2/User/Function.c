#include "Function.h"

char text[40]="";
u16 Task_Timers[Task_MAX]={0};
PRO_DATA pro_data={0};
KEYS keys[4]={0};
IC PB4_TIM3={0};


void LedOut(uint16_t GPIO_Pin, u8 PinState)
{
	static u16 led_state=0xff00;
	if(PinState==1){
		led_state|=GPIO_Pin&0xff00;
	}else{
		led_state&=(~GPIO_Pin)&0xff00;
	}
	GPIOC->ODR=led_state;
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
}

double Get_ADC_Value(void)
{
	HAL_ADC_Start(&hadc2);
	double val=HAL_ADC_GetValue(&hadc2);
	return val/4096.0f*3.3f;
}

void SysTick_Handler(void)
{
  HAL_IncTick();
	fors(u8 i=0;i<Task_MAX;i++)
	{
		if(Task_Timers[i])
			Task_Timers[i]--;
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM6)
	{
		keys[0].state=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0);
		keys[1].state=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1);
		keys[2].state=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2);
		keys[3].state=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0);

		fors(u8 i=0;i<4;i++)
		{
			if(keys[i].cnt>=2&&keys[i].state==1){
				keys[i].flag=1;
			}else if(keys[i].cnt>=80&&keys[i].state==1){
				keys[i].flag=2;
			}else if(keys[i].cnt>=80&&keys[i].state==0){
				keys[i].flag=3;
			}
			
			if(keys[i].state==0){
				keys[i].cnt++;
			}else{
				keys[i].cnt=0;
			}
		}
	}
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM3)
	{
		if(htim->Channel==HAL_TIM_ACTIVE_CHANNEL_1)
		{
			if(PB4_TIM3.mode==1)
				{
					PB4_TIM3.CCR1[0]=TIM3->CCR1;
					PB4_TIM3.CCR2[0]=TIM3->CCR1;
				}
			else
				{
					PB4_TIM3.CCR1[1]=TIM3->CCR1;
					PB4_TIM3.CCR2[1]=TIM3->CCR2;
					TIM3->CNT=0;
					PB4_TIM3.frq=1000000.0/(double)(PB4_TIM3.CCR1[1]-PB4_TIM3.CCR1[0]);
					PB4_TIM3.duty=(double)(PB4_TIM3.CCR2[1]-PB4_TIM3.CCR2[0])/(double)(PB4_TIM3.CCR1[1]-PB4_TIM3.CCR1[0])*100.0;
			}
			PB4_TIM3.mode=!PB4_TIM3.mode;
			HAL_TIM_IC_Start(htim,TIM_CHANNEL_1);
		}
	}
}


