/*
	该文件用于存放底层函数驱动代码，包括自制函数与回调函数.
*/
#ifndef __FUNCTION_h_
#define __FUNCTION_h_

#include "Config.h"

typedef struct{
	char menu[30];
	int Rate,Rate_last;
	int Con;
	int Max,Min;
	int High,Low;
	int High_temp,Low_temp;

}PRO_DATA;
extern PRO_DATA pro_data;

extern char text[40];

#define Task_MAX 7
extern u16 Task_Timers[Task_MAX];
#define LCD_Task_Timer Task_Timers[0]
#define KEY_Task_Timer Task_Timers[1]
#define LED_Task_Timer Task_Timers[2]
#define Fre_dataTask_Timer Task_Timers[3]
#define ADC_dataTask_Timer Task_Timers[4]
#define Statistics_Alarms_Task_Timer Task_Timers[5]
#define _5000ms_Tiemr	 Task_Timers[6]
typedef struct{
	u8 state;
	u32 cnt;
	u8 flag;
}KEYS;
extern KEYS keys[4];

#define LED1 GPIO_PIN_8
#define LED2 GPIO_PIN_9
#define LED3 GPIO_PIN_10
#define LED4 GPIO_PIN_11
#define LED5 GPIO_PIN_12
#define LED6 GPIO_PIN_13
#define LED7 GPIO_PIN_14
#define LED8 GPIO_PIN_15

void LedOut(uint16_t GPIO_Pin, u8 PinState);

typedef struct{
	double frq,duty;
	u32 CCR1[2],CCR2[2];
	u8 mode;
}IC;
extern IC PB4_TIM3;

double Get_ADC_Value(void);

#endif
