#include "config.h"
#include "task.h"
u8 LCD_Flag=0;
void Task_Init()
{
	pro_data.High_temp=100;
	pro_data.Low_temp=60;
	
	pro_data.High=100;
	pro_data.Low=60;
	LCD_Flag=0;
}
void LCD_DisTaks(u16 Timer)
{
		if(LCD_Task_Timer)return;
			LCD_Task_Timer=Timer;
	if(LCD_Flag==0)
	{
		//data页面
		char LCD_Text1[30];
		LCD_DisplayStringLine(Line1,(unsigned char *)"       HEART       ");
		
		sprintf(LCD_Text1,"       Rate:%d  ",pro_data.Rate);
		LCD_DisplayStringLine(Line3,(u8 *)LCD_Text1);
		
		sprintf(LCD_Text1,"        Con:%d  ",pro_data.Con);
		LCD_DisplayStringLine(Line4,(u8 *)LCD_Text1);
		
		if(!strstr(pro_data.menu,"exit_set_para"))
		{
			sprintf(pro_data.menu,"out_set_para");
			pro_data.High_temp=pro_data.High;
			pro_data.Low_temp=pro_data.Low;
			
		}
		
	}
	else if(LCD_Flag==1)
	{
		//record页面
		char LCD_Text2[30];
		LCD_DisplayStringLine(Line1,(unsigned char *)"       RECORD       ");
		
		sprintf(LCD_Text2,"       Max:%d      ",pro_data.Max);
		LCD_DisplayStringLine(Line3,(u8 *)LCD_Text2);
		
		sprintf(LCD_Text2,"       Min:%d      ",pro_data.Min);
		LCD_DisplayStringLine(Line4,(u8 *)LCD_Text2);
		pro_data.High=pro_data.High_temp;
		pro_data.Low=pro_data.Low_temp;
	}
	else if(LCD_Flag==2)
	{
		//para界面
		char LCD_Text3[30];
		LCD_DisplayStringLine(Line1,(unsigned char *)"       PARA       ");
		
		sprintf(LCD_Text3,"       High:%d      ",pro_data.High_temp);
		LCD_DisplayStringLine(Line3,(u8 *)LCD_Text3);
		
		sprintf(LCD_Text3,"       Low:%d       ",pro_data.Low_temp);
		LCD_DisplayStringLine(Line4,(u8 *)LCD_Text3);
	}
		sprintf(text,"State:%s   ",pro_data.menu);
	LCD_DisplayStringLine(Line9,(u8 *)text);
}
void Key_Taks(u16 Timer)
{
	if(KEY_Task_Timer)return;
		KEY_Task_Timer=Timer;
	if(keys[0].flag==1||keys[0].flag==2)
	{
		LCD_Flag++;
		if(LCD_Flag>2)LCD_Flag=0;
		
		if(strstr(pro_data.menu,"out_set_para"))
		{
			if(LCD_Flag==1)
			{
				sprintf(pro_data.menu,"set_statepara");
			}
		}
	}
	else if(keys[1].flag==1||keys[1].flag==2)
	{
			if(strstr(pro_data.menu,"set_high"))
			sprintf(pro_data.menu,"set_low");
			
			else if(strstr(pro_data.menu,"set_low"))
			sprintf(pro_data.menu,"set_high");
			

	}
	else if(keys[2].flag==1||keys[2].flag==2)
	{
			
			if(strstr(pro_data.menu,"set_statepara")||strstr(pro_data.menu,"exit_set_para"))
			{
				sprintf(pro_data.menu,"set_high");
			}
			else if(strstr(pro_data.menu,"set_high")||strstr(pro_data.menu,"set_low"))
			sprintf(pro_data.menu,"exit_set_para");
				
	}
	else if (keys[3].flag==1||keys[3].flag==2)
	{
		if(LCD_Flag==0)
		pro_data.Con=0;
	}
	fors(u8 i=0;i<4;i++)
	{
		keys[i].flag=0;
	}
}
void Fre_dataTask(u16 Timers)
{
	if(Fre_dataTask_Timer)return;
	Fre_dataTask_Timer=Timers;
	if(PB4_TIM3.frq>2000)pro_data.Rate=200;
	else if(PB4_TIM3.frq<1000)pro_data.Rate=30;
	else pro_data.Rate=(170.0f/1000.0f)*(PB4_TIM3.frq-1000.0f)+30;
}
void ADC_dataTask(u16 Timers)
{
	double ADC_RealValue=0.0;
	if(ADC_dataTask_Timer)return;
		ADC_dataTask_Timer=Timers;
	ADC_RealValue=Get_ADC_Value();
	if(strstr(pro_data.menu,"set_high")&&LCD_Flag==2)
	{
			if(ADC_RealValue<1.00f)
			{
				pro_data.High_temp=60;
			}
			else if(ADC_RealValue>3.00f)
			{
				pro_data.High_temp=150;
			}
			else
			{
				pro_data.High_temp=(150.0f-60.0f)/2.0f*(ADC_RealValue-1.00f)+60;
			}
	}
		else if(strstr(pro_data.menu,"set_low")&&LCD_Flag==2)
		{
			if(ADC_RealValue<1.00f)
			{
				pro_data.Low_temp=60;
			}
			else if(ADC_RealValue>3.00f)
			{
				pro_data.Low_temp=150;
			}
			else
			{
				pro_data.Low_temp=(150.0f-60.0f)/2.0f*(ADC_RealValue-1.00f)+60;
			}
		}
}
void LED_Task(u16 Timer)
{
	if(LED_Task_Timer)return;
		LED_Task_Timer=Timer;
	if(LCD_Flag==0)LedOut(LED1,0);
	else LedOut(LED1,1);
	
	if(LCD_Flag==1)LedOut(LED2,0);
	else LedOut(LED2,1);
	
	if(LCD_Flag==2)LedOut(LED3,0);
	else LedOut(LED3,1);
	
	if(strstr(pro_data.menu,"set_high"))LedOut(LED4,0);
	else LedOut(LED4,1);
	
	if(strstr(pro_data.menu,"set_low"))LedOut(LED5,0);
	else LedOut(LED5,1);
	if(LCD_Flag==0)
	{
		if(_5000ms_Tiemr)
		{
			LedOut(LED6,0);
		}
		else
		{
			LedOut(LED6,1);

		}
	}
	
}
void Statistics_Alarms_Task(u32 Timer)
{
	if(Statistics_Alarms_Task_Timer)return;
	Statistics_Alarms_Task_Timer=Timer;
	
	//记录功能
	static u8 flag1=0;
	if(flag1==0)
	{
		pro_data.Min=pro_data.Rate;
		flag1=1;
	}
	else
	{
		if(pro_data.Rate>pro_data.Max)
		{
			pro_data.Max=pro_data.Rate;
		}
		else 	if(pro_data.Rate<pro_data.Min)
		{
			pro_data.Min=pro_data.Rate;

		}
	}
	
	//报警功能
	static u8 flag2=0;
	if(flag2==0)
	{
		pro_data.Rate_last=75;
		flag2=1;
	}

	if(pro_data.Rate>pro_data.Rate_last )
	{
		if(! (pro_data.Rate_last>pro_data.High_temp) )//非 心率一直大于上限报警
			if(pro_data.Rate>pro_data.High_temp)
			{
				pro_data.Con++;
				_5000ms_Tiemr=5000;
			}
	}
	
	else if(pro_data.Rate<pro_data.Rate_last)
	{
		if(! (pro_data.Rate_last<pro_data.Low_temp) )//非 心率一直小于下限报警
			if(pro_data.Rate<pro_data.Low_temp)
			{
				pro_data.Con++;
				_5000ms_Tiemr=5000;
			}
	}
	pro_data.Rate_last=pro_data.Rate;
}
void Task_run(void)
{
	LCD_DisTaks(50);
	LED_Task(100);
	Key_Taks(10);
	Fre_dataTask(100);
	ADC_dataTask(100);
	Statistics_Alarms_Task(100);
}
