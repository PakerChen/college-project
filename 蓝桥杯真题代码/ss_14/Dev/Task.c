#include "config.h"
#include "task.h"
uint8_t LCD_UIFlag=1;
uint8_t Data_ModeFlag=0;//0表示高频，1表示低频
uint8_t change_ModeCount=0;
uint8_t B2_DalayFlag=0;
uint8_t ChangeMode_Flag=1;
uint8_t K_Value=1;
uint8_t R_Value=1;
uint8_t RValue_ControlFlag,KValue_ControlFlag=0;
uint8_t P_Value=0;
float Votagle=0.0;
float V_Value=0.0;
float MH_Value,ML_Value=0.0;
float temp_MH_Value=0.0;
float temp_ML_Value=0.0;
uint8_t Duty_Control=0;
static uint16_t B2_Dalaycount=0;
uint8_t Task_Flag=1;
uint32_t PA1_Fre=0;
uint8_t R_Valuefirst=0;
uint16_t psc_Value;
void LCD_Task(uint16_t Timer)
{
	if(LCD_Task_Timer)return;
	LCD_Task_Timer=Timer;
	if(LCD_UIFlag==1){dis_Data_Menu();}
	else if(LCD_UIFlag==2){dis_para_menu();}
	else if(LCD_UIFlag==3){dis_recd_menu();}
}
void dis_Data_Menu(void)
{
	char text3[30]={0};
	LCD_DisplayStringLine(Line1,(unsigned char *)"        DATA  ");
	if(Data_ModeFlag==0&&B2_DalayFlag==0)
	{LCD_DisplayStringLine(Line3,(unsigned char *)"     M=H");}
	else if(Data_ModeFlag==1&&B2_DalayFlag==0)
	{LCD_DisplayStringLine(Line3,(unsigned char *)"     M=L");}
	
	sprintf(text3,"     P=%d%%     ",P_Value);
	LCD_DisplayStringLine(Line4,(u8 *)text3);
	
	V_Value=((PA7_Fre*2*3.14*R_Value)/(100*K_Value));
	sprintf(text3,"     V=%.2f    ",V_Value);
	LCD_DisplayStringLine(Line5,(u8 *)text3);
	
//	PA1_Fre=80000000/(htim2.Instance->PSC*htim2.Instance->ARR);
//	
//			sprintf(text3,"     Task_Flag=%d    ",Task_Flag);
//	LCD_DisplayStringLine(Line6,(u8 *)text3);
//	
//				sprintf(text3,"     PA1_Fre=%d    ",PA1_Fre);
	
//	LCD_DisplayStringLine(Line7,(u8 *)text3);
//		sprintf(text3,"     PA7_Fre=%d    ",PA7_Fre);
//	LCD_DisplayStringLine(Line7,(u8 *)text3);
//			sprintf(text3,"     B2_count=%d    ",B2_Dalaycount);
//		LCD_DisplayStringLine(Line0,(u8 *)text3);
//			sprintf(text3,"    TIM3_Duty=%d    ",TIM3_Duty);
//		LCD_DisplayStringLine(Line6,(u8 *)text3);
//////	
////	sprintf(text3,"     Votagle=%.2f    ",Votagle);
////	LCD_DisplayStringLine(Line7,(u8 *)text3);
////	
////		sprintf(text3,"     key_3=%d    ",key_presstime[3]);
////	LCD_DisplayStringLine(Line8,(u8 *)text3);
////	
////		sprintf(text3,"     key_value=%d    ",key_value);
////	LCD_DisplayStringLine(Line9,(u8 *)text3);

//		sprintf(text3,"     psc_value=%d    ", psc_Value);
//	LCD_DisplayStringLine(Line9,(u8 *)text3);
}
void dis_para_menu(void)
{
	char text1[30]={0};
	LCD_DisplayStringLine(Line1,(unsigned char *)"        PARA  ");	
	sprintf(text1,"    R=%d     ",R_Value);
	LCD_DisplayStringLine(Line3,(u8 *)text1);
	
	sprintf(text1,"    K=%d     ",K_Value);
	LCD_DisplayStringLine(Line4,(u8 *)text1);
	LCD_DisplayStringLine(Line5,(unsigned char *)"                ");	
}
void dis_recd_menu(void)
{
	char text2[30]={0};
	LCD_DisplayStringLine(Line1,(unsigned char *)"        RECD");
	
	sprintf(text2,"    N=%d     ",change_ModeCount);
	LCD_DisplayStringLine(Line3,(u8 *)text2);
	
	sprintf(text2,"    MH=%.1f    ",MH_Value);
	LCD_DisplayStringLine(Line4,(u8 *)text2);
	
	sprintf(text2,"    ML=%.1f    ",ML_Value);
	LCD_DisplayStringLine(Line5,(u8 *)text2);
}

void Key_Task(uint16_t Timer)
{
	if(Key_Task_Timer)return;
	Key_Task_Timer=Timer;
	if(key_value==1)//按键1按下
	{
		if(LCD_UIFlag==1)
		{
			LCD_UIFlag=2;
			if(RValue_ControlFlag==0)
			{
				RValue_ControlFlag=1;
				KValue_ControlFlag=0;
			}
		}
		else if(LCD_UIFlag==2){LCD_UIFlag=3;}
		else if(LCD_UIFlag==3){LCD_UIFlag=1;}
		key_value=0;
	}
	else if(key_value==2)//B2按下
	{
		if(LCD_UIFlag==1)
		{
			if(ChangeMode_Flag==1)
			{
				B2_DalayFlag=1;//开始延时
				change_ModeCount+=1;
				ChangeMode_Flag=0;
				if(Data_ModeFlag==0){Data_ModeFlag=1;}
				else if(Data_ModeFlag==1){Data_ModeFlag=0;}
			}
		}
		key_value=0;
	}
	else if (key_value==3)
	{
		if(LCD_UIFlag==2)
		{
			if(RValue_ControlFlag==1)
			{
				R_Value+=1;
				if(R_Value>10){R_Value=1;}
			}
			else if(KValue_ControlFlag==1)
			{
				K_Value+=1;
				if(K_Value>10){K_Value=1;}
			}
		}
		key_value=0;
	}
	else if(key_value==4)
	{
		if(LCD_UIFlag==2)
		{
			if(RValue_ControlFlag==1)
			{
				R_Value-=1;
				if(R_Value<1){R_Value=10;}
				R_Valuefirst=0;
			}
			else if(KValue_ControlFlag==1)
			{
				K_Value-=1;
				if(K_Value<1){K_Value=10;}
			}
		}
		key_value=0;
	}
}
void B2_DelayTask(uint16_t Timer)
{
	if(B2_DelayTaskTimer)return;
	B2_DelayTaskTimer=Timer;

	if(B2_DalayFlag==1&&++B2_Dalaycount==2000)//开始延时5秒
	{
		B2_DalayFlag=0;
		B2_Dalaycount=0;
		ChangeMode_Flag=1;
	}
}
void LCD_KeyTask(uint16_t Timer)
{
	if(LCD_KeyTaskTimer)return;
	LCD_KeyTaskTimer=Timer;
	if(LCD_UIFlag==2)
	{
		if(key_value==2)
		{
			if(RValue_ControlFlag==1)
				{
					RValue_ControlFlag=0;
					KValue_ControlFlag=1;
				}
				else if(KValue_ControlFlag==1)
				{
					RValue_ControlFlag=1;
					KValue_ControlFlag=0;
				}
		}
	}	
	if(LCD_UIFlag==1)
	{
		if(key_value==4)
		B4_LockFlag=0;
		
	}
}
void LED_Task(uint16_t Timers)
{
	if(LED_TaskTimers)return;
	LED_TaskTimers=Timers;
	if(LCD_UIFlag==1)
	{
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12
						|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
	}
	else if (LCD_UIFlag==2||LCD_UIFlag==3)
	{
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12
						|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
	}
	if(B4_LockFlag==1)
	{
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11|GPIO_PIN_12
						|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);	
	}
}
void Fre_Task(uint16_t Timers)
{
	if(Fre_TaskTimers)return;
	Fre_TaskTimers=Timers;
	psc_Value=htim2.Instance->PSC;
	uint8_t Flag=0;

	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
	uint32_t TIM3_period = htim2.Init.Period + 1;  // ARR + 1
	uint32_t TIM3_pulse = htim2.Instance->CCR1;    // CCR1
	TIM3_Duty=((float)TIM3_pulse / (float)TIM3_period) * 100;  // 计算占空比
	if(LCD_UIFlag==1)
	{
		if(Data_ModeFlag==0)
		{
			//高频
			if(TIM2->PSC!=100-1)
			{
				TIM2->PSC-=10;
			}
		}
		else if(Data_ModeFlag==1)
		{
			//低频
			if(TIM2->PSC!=200-1)
			{
				TIM2->PSC+=10;
			}
		}
		if(B4_LockFlag==0)
		{
			P_Value=Votagle*37.5-27.5;
			if(Votagle>3.00)
			{
				TIM2->CCR2=85;
				P_Value=85;
			}
			else if(Votagle<1.00)
			{
				TIM2->CCR2=10;
				P_Value=10;
			}
			else 
				TIM2->CCR2=P_Value;
		}
	}
	if(LCD_UIFlag==3)
	{
		//计算最大值和最小值
		if(Data_ModeFlag==0)//高频
		{		
			temp_MH_Value=((PA7_Fre*2*3.14*R_Value)/(100*K_Value));
			if(temp_MH_Value>MH_Value)
			{
				MH_Value=temp_MH_Value;
			}
		}
		else if(Data_ModeFlag==1)
		{
			temp_ML_Value=((PA7_Fre*2*3.14*R_Value)/(100*K_Value));
			if(temp_ML_Value>ML_Value)
			{
				ML_Value=temp_ML_Value;
			}
		}
	}
}
void adc_getvalueTask(uint16_t Timers)
{
	if(adc_getvalueTaskTimers)return;
	adc_getvalueTaskTimers=Timers;
		uint16_t temp = 0;
    uint32_t sum = 0; // 用来存储5次采样的总和
    float average = 0;

    // 执行5次ADC读取
    for (int i = 0; i < 10; i++) {
        HAL_ADC_Start(&hadc2);                  // 启动ADC
        temp = HAL_ADC_GetValue(&hadc2);        // 读取ADC采集值
        HAL_ADC_Stop(&hadc2);                   // 关闭ADC
        sum += temp;  // 将每次读取的结果加到总和中
    }
    // 计算平均值
    average = (float)sum / 10.0;
    // 转换为电压值，并返回
    Votagle=(average / 4095.0) * 3.30;  // 假设最大ADC值为4095，参考电压为3.30V
}
void B2_DelayLED(uint16_t Timers)
{
	Task_Flag=1;
	if(B2_DelayLEDTimers)return;
	B2_DelayLEDTimers=Timers;
	if(B2_DalayFlag==1)
	{
		
		if(Task_Flag==1)
		{
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
  			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12
							|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15,GPIO_PIN_SET);
			if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_9)==1)
			{
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_RESET);
			}
			else if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_9)==0)
			{
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_SET);
			}
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
			
		}
		Task_Flag=0;
	}
}
void Task_run(void)
{
	Key_Task(10);
	LCD_Task(50);
	B2_DelayTask(1);
	LCD_KeyTask(10);
	LED_Task(100);
	Fre_Task(100);
	adc_getvalueTask(10);
	B2_DelayLED(100);
}
