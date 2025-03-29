#ifndef __task_H__
#define __task_H__

#include "ht32.h"
#ifdef __cplusplus
 extern "C" {
#endif

typedef struct timer_parameter {
    void (*timeout_cb)(void); 
	uint32_t timeout;
	uint32_t repeat;
}task_timer_parameter;


int task_init(void);
int task_run(void);
#define ChuanKouPin_Printf UART0_printf
#define YouXian_Printf printf
#define Wifi_Printf u0_printf
#define BlueT_Printf USART1_printf
#ifdef __cplusplus
}
#endif


#endif
/*****************************END OF FILE*********************************/
