#include "config.h"
#include "function.h"
uint16_t Task_Timers[Task_MAX]={0};
uint16_t key_presstime[4]={0};
uint8_t key_value=0;
uint8_t B4_LockFlag=0;
void SysTick_Handler(void)
{
	HAL_IncTick();
	for(uint8_t i=0;i<Task_MAX;i++)
	{
		if(Task_Timers[i])
			Task_Timers[i]--;
	}
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM6)
	{
		if(!HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)){key_presstime[0]++;}
		else if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)&&key_presstime[0]>5){key_presstime[0]=0;key_value=1;}
		else if(!HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)){key_presstime[1]++;}
		else if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)&&key_presstime[1]>5){key_presstime[1]=0;key_value=2;}
		else if(!HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)){key_presstime[2]++;}
		else if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)&&key_presstime[2]>5){key_presstime[2]=0;key_value=3;}
		else if(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)){key_presstime[3]++;}
		
		else if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)&&key_presstime[3]>200){key_presstime[3]=0;B4_LockFlag=1;}
		else if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)&&key_presstime[3]>5&&key_presstime[3]<200){key_presstime[3]=0;key_value=4;}
		else{key_presstime[0]=0;key_presstime[1]=0;key_presstime[2]=0;key_presstime[3]=0;}
	}
}
uint32_t PA7_Fre=0;
uint32_t TIM3_Duty=0;
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
		uint32_t  cc1_value=0;
		cc1_value = __HAL_TIM_GET_COUNTER(htim);
		__HAL_TIM_SetCounter(htim,0);

		if(htim->Instance==TIM3)
		{
			PA7_Fre = 1000000/cc1_value;
			
		}
}
