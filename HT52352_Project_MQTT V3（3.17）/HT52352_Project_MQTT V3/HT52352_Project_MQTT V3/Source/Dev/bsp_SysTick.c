#include "config.h"
#include "bsp_SysTick.h"

unsigned long 			sysTimer[MAX_TIMER_SYSTICK];		//定义系统定时器 
static unsigned long  g_iRunTime = 0;//定义全局运行时间

void SysTickInit(void)
{
	unsigned char  i;
	//清零所有定时器
	for (i = 0; i < MAX_TIMER_SYSTICK; i++)	  			
		sysTimer[i]= 0 ;	
	
//	SysTick_Config(48000000/1000);// 每计数48000进入滴答定时器中断，主频为48MS，故1ms进入1次滴答中断
	/* SYSTICK configuration */
	SYSTICK_ClockSourceConfig(SYSTICK_SRC_STCLK);       // Default : CK_SYS/8  (6MHZ)
	SYSTICK_SetReloadValue(SystemCoreClock / 8 / 1000); // (CK_SYS/8/1000) = 1ms on chip
	SYSTICK_IntConfig(ENABLE);                          // Enable SYSTICK Interrupt
	
	SYSTICK_CounterCmd(SYSTICK_COUNTER_CLEAR);
	SYSTICK_CounterCmd(SYSTICK_COUNTER_ENABLE);//启动系统滴答

}

void SysTick_Handler(void)
{
	unsigned char  i;
	
	for (i = 0; i < MAX_TIMER_SYSTICK ; i++)				//定时时间递减
		if( sysTimer[i] ) 							//直到定时时间为0为止
			sysTimer[i]-- ;		

  // 全局运行计数时间处理	
	if (g_iRunTime++ == 0xFFFFFFFF)							// 计数溢出清0	约50天
	{
		g_iRunTime = 0;
	}
	timer_ticks(); //1ms ticks
}

void SysTickDelayMs(unsigned long  nms)
{

  DelayTimer = nms;

  while(DelayTimer != 0);
	
}
void SysTickDelayUs(unsigned long  nus)//微妙延时，只针对1ms以内的延时
{
	u32 delta,current_delay;

    /* 获得延时经过的tick数 */
    nus = nus * (SysTick->LOAD/(1000000/1000));

    /* 获得当前时间 */
    delta = SysTick->VAL;
	
	do
    {
		if ( delta > SysTick->VAL )
			current_delay = delta - SysTick->VAL;
		else
        /* 延时跨越边界时的处理 */
			current_delay = SysTick->LOAD + delta - SysTick->VAL;
    } while( current_delay < nus );
}
/*******************************************************************************************************
** 函数: GetRunTime, 获取CPU运行时间，单位1ms
**------------------------------------------------------------------------------------------------------
** 参数: 无
** 返回: CPU运行时间，单位1ms
********************************************************************************************************/
unsigned long  GetRunTime(void)
{
	unsigned long  RunTime; 

//	CPU_INT_DISABLE();								// 关中断 
	RunTime = g_iRunTime;							// 由于在Systick中断被改写，因此关中断进行保护 
//	CPU_INT_ENABLE();								// 开中断 

	return RunTime;
}
/***************************************************************************************
***************************************end of file**************************************
***************************************************************************************/
