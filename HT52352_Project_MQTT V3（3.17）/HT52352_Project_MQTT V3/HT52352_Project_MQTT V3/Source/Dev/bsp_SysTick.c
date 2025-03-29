#include "config.h"
#include "bsp_SysTick.h"

unsigned long 			sysTimer[MAX_TIMER_SYSTICK];		//����ϵͳ��ʱ�� 
static unsigned long  g_iRunTime = 0;//����ȫ������ʱ��

void SysTickInit(void)
{
	unsigned char  i;
	//�������ж�ʱ��
	for (i = 0; i < MAX_TIMER_SYSTICK; i++)	  			
		sysTimer[i]= 0 ;	
	
//	SysTick_Config(48000000/1000);// ÿ����48000����δ�ʱ���жϣ���ƵΪ48MS����1ms����1�εδ��ж�
	/* SYSTICK configuration */
	SYSTICK_ClockSourceConfig(SYSTICK_SRC_STCLK);       // Default : CK_SYS/8  (6MHZ)
	SYSTICK_SetReloadValue(SystemCoreClock / 8 / 1000); // (CK_SYS/8/1000) = 1ms on chip
	SYSTICK_IntConfig(ENABLE);                          // Enable SYSTICK Interrupt
	
	SYSTICK_CounterCmd(SYSTICK_COUNTER_CLEAR);
	SYSTICK_CounterCmd(SYSTICK_COUNTER_ENABLE);//����ϵͳ�δ�

}

void SysTick_Handler(void)
{
	unsigned char  i;
	
	for (i = 0; i < MAX_TIMER_SYSTICK ; i++)				//��ʱʱ��ݼ�
		if( sysTimer[i] ) 							//ֱ����ʱʱ��Ϊ0Ϊֹ
			sysTimer[i]-- ;		

  // ȫ�����м���ʱ�䴦��	
	if (g_iRunTime++ == 0xFFFFFFFF)							// ���������0	Լ50��
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
void SysTickDelayUs(unsigned long  nus)//΢����ʱ��ֻ���1ms���ڵ���ʱ
{
	u32 delta,current_delay;

    /* �����ʱ������tick�� */
    nus = nus * (SysTick->LOAD/(1000000/1000));

    /* ��õ�ǰʱ�� */
    delta = SysTick->VAL;
	
	do
    {
		if ( delta > SysTick->VAL )
			current_delay = delta - SysTick->VAL;
		else
        /* ��ʱ��Խ�߽�ʱ�Ĵ��� */
			current_delay = SysTick->LOAD + delta - SysTick->VAL;
    } while( current_delay < nus );
}
/*******************************************************************************************************
** ����: GetRunTime, ��ȡCPU����ʱ�䣬��λ1ms
**------------------------------------------------------------------------------------------------------
** ����: ��
** ����: CPU����ʱ�䣬��λ1ms
********************************************************************************************************/
unsigned long  GetRunTime(void)
{
	unsigned long  RunTime; 

//	CPU_INT_DISABLE();								// ���ж� 
	RunTime = g_iRunTime;							// ������Systick�жϱ���д����˹��жϽ��б��� 
//	CPU_INT_ENABLE();								// ���ж� 

	return RunTime;
}
/***************************************************************************************
***************************************end of file**************************************
***************************************************************************************/
