#ifndef __TASK_h_
#define __TASK_h_

extern uint8_t RValue_ControlFlag,KValue_ControlFlag;
extern uint8_t Data_ModeFlag;
void dis_Data_Menu(void);
void dis_para_menu(void);
void dis_recd_menu(void);
void Key_Task(uint16_t Timer);
void Task_run(void);
#endif
