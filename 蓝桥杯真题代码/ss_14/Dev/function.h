#ifndef __FUNCTION_h_
#define __FUNCTION_h_


typedef struct{
	uint8_t state;
	uint32_t cnt;
	uint8_t flag;
}KEYS;
extern KEYS keys[4];



extern uint8_t key_value;
extern uint32_t PA7_Fre;
extern uint8_t B4_LockFlag;
extern uint16_t key_presstime[4];
extern uint8_t key_value;
extern uint32_t TIM3_Duty;
#define Task_MAX 8
extern uint16_t Task_Timers[Task_MAX];
#define LCD_Task_Timer Task_Timers[0]
#define Key_Task_Timer Task_Timers[1]
#define B2_DelayTaskTimer Task_Timers[2]
#define LCD_KeyTaskTimer Task_Timers[3]
#define LED_TaskTimers Task_Timers[4]
#define Fre_TaskTimers Task_Timers[5]
#define adc_getvalueTaskTimers Task_Timers[6]
#define B2_DelayLEDTimers Task_Timers[7]
#endif
