#include "bsp_i2c.h"
#include "config.h"
#define I2C_SCLK_PIN    GPIO_PIN_14    
#define I2C_MOIS_PIN    GPIO_PIN_15  

#define	I2C_MOIS_OUT_H HT_GPIOC->SRR = GPIO_PIN_15
#define	I2C_MOIS_OUT_L HT_GPIOC->RR = GPIO_PIN_15
#define	I2C_MOIS_IN  GPIO_ReadInBit(HT_GPIOC,GPIO_PIN_15)

#define I2C_SCL_OUT_H  HT_GPIOC->SRR = GPIO_PIN_14
#define I2C_SCL_OUT_L  HT_GPIOC->RR = GPIO_PIN_14
/*******************************************************************************************************
** 函数: DelayAt24c02, SPI延时函数
**------------------------------------------------------------------------------------------------------
** 参数:  ms 时间 
** 返回: void
*******************************************************************************************************/
static void DelayAt24c02(unsigned long ms)
{	
	while(ms--);
}

/*******************************************************************************************************
** 函数: I2c_GpioInit 初始化
**------------------------------------------------------------------------------------------------------
** 参数: void 
** 返回: void
*******************************************************************************************************/
void I2c_GpioInit(void)
{
		CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
    CKCUClock.Bit.PC         = 1;
    CKCUClock.Bit.AFIO       = 1;
    CKCUClock.Bit.BKP        = 1;
    CKCU_PeripClockConfig(CKCUClock, ENABLE);

		AFIO_GPxConfig(GPIO_PC, I2C_SCLK_PIN, AFIO_FUN_GPIO);
		GPIO_DirectionConfig(HT_GPIOC, I2C_SCLK_PIN, GPIO_DIR_OUT);	 
		AFIO_GPxConfig(GPIO_PC, I2C_MOIS_PIN, AFIO_FUN_GPIO);
		GPIO_DirectionConfig(HT_GPIOC, I2C_MOIS_PIN, GPIO_DIR_OUT);
		I2cStop();
}


/*******************************************************************************************************
** 函数: I2C_MOSI_OUT 初始化
**------------------------------------------------------------------------------------------------------
** 参数: void 
** 返回: void
*******************************************************************************************************/
void I2C_MOSI_OUT(void)
{
		CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
    CKCUClock.Bit.PC         = 1;
    CKCUClock.Bit.AFIO       = 1;

    CKCU_PeripClockConfig(CKCUClock, ENABLE);
		AFIO_GPxConfig(GPIO_PC, I2C_MOIS_PIN, AFIO_FUN_GPIO);
		GPIO_DirectionConfig(HT_GPIOC, I2C_MOIS_PIN, GPIO_DIR_OUT);
}

/*******************************************************************************************************
** 函数: I2C_MOSI_IN 初始化
**------------------------------------------------------------------------------------------------------
** 参数: void 
** 返回: void
*******************************************************************************************************/
void I2C_MOSI_IN(void)
{
	  CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
    CKCUClock.Bit.PC         = 1;
    CKCUClock.Bit.AFIO       = 1;
    CKCU_PeripClockConfig(CKCUClock, ENABLE);

		AFIO_GPxConfig(GPIO_PC, I2C_MOIS_PIN, AFIO_FUN_GPIO);
		GPIO_DirectionConfig(HT_GPIOC, I2C_MOIS_PIN, GPIO_DIR_IN);
		GPIO_InputConfig(HT_GPIOC,I2C_MOIS_PIN,ENABLE);  
}

/*******************************************************************************************************
** 函数: I2cStart,I2C开始信号
**------------------------------------------------------------------------------------------------------
** 参数: void
** 返回: void
********************************************************************************************************/
void I2cStart(void)
{	
	I2C_MOSI_OUT();
	I2C_MOIS_OUT_H;
	I2C_SCL_OUT_H ;
	SysTickDelayUs(2);
	I2C_MOIS_OUT_L;
	SysTickDelayUs(2);
	I2C_SCL_OUT_L ;
	SysTickDelayUs(2);
}
/*******************************************************************************************************
** 函数: I2cStop,I2C停止信号
**------------------------------------------------------------------------------------------------------
** 参数: void
** 返回: void
********************************************************************************************************/
void I2cStop( void )
{	
	I2C_MOSI_OUT();
	I2C_MOIS_OUT_L;
	I2C_SCL_OUT_L ;
	SysTickDelayUs(2);
	I2C_MOIS_OUT_H;
	I2C_SCL_OUT_H ;
	SysTickDelayUs(2);
}
/*******************************************************************************************************
** 函数: I2cACK,应答读信号
**------------------------------------------------------------------------------------------------------
** 参数: void
** 返回: void
********************************************************************************************************/
void I2cACK(void)
{	
	I2C_MOSI_OUT();
	I2C_SCL_OUT_L ;
	I2C_MOIS_OUT_L;
	I2C_SCL_OUT_H;
	SysTickDelayUs(1);
	I2C_SCL_OUT_L;
	I2C_MOIS_OUT_H; 
}
/*******************************************************************************************************
** 函数: I2cNoACK,非应答信号
**------------------------------------------------------------------------------------------------------
** 参数: void
** 返回: void
********************************************************************************************************/
void I2cNoACK(void)
{	
	I2C_MOSI_OUT();
	I2C_SCL_OUT_L ;
	I2C_MOIS_OUT_H;
	I2C_SCL_OUT_H ;
	SysTickDelayUs(1);
	I2C_SCL_OUT_L ;
}

/*******************************************************************************************************
** 函数: I2cRACK,读取应答信号
**------------------------------------------------------------------------------------------------------
** 参数: void
** 返回: 有应答=1,无应答=0
********************************************************************************************************/
int I2cRACK(void) 	 
{
	int bat=0;
	I2C_MOSI_OUT();
	I2C_SCL_OUT_L ; 
	I2C_MOIS_OUT_H;			
	I2C_SCL_OUT_H ;
	I2C_MOSI_IN();
	SysTickDelayUs(1);
	bat=I2C_MOIS_IN;
	I2C_SCL_OUT_L ; 
	return bat;
}


/*******************************************************************************************************
** 函数: I2cWriteByte,写一字节数据
**------------------------------------------------------------------------------------------------------
** 参数: b 数据
** 返回: void
********************************************************************************************************/
void I2cWriteByte(unsigned char b)
{	unsigned char i=0;
	I2C_MOSI_OUT();
	for(i=0;i<8;i++)
	{	
		if(b&0x80 ? 1:0)I2C_MOIS_OUT_H;	
		else I2C_MOIS_OUT_L;	
		I2C_SCL_OUT_H ;
		SysTickDelayUs(2);
		I2C_SCL_OUT_L ;
		b<<=1;
	}
}

/*******************************************************************************************************
** 函数: I2cReadByte,读取一字节
**------------------------------------------------------------------------------------------------------
** 参数: dat 数据指针
** 返回: void
********************************************************************************************************/
void I2cReadByte(unsigned char *dat)
{	unsigned char i;
	I2C_MOSI_IN();
	for(i=0;i<8;i++)
	{	
		*dat<<=1; 
		I2C_SCL_OUT_H;
		DelayAt24c02(1);
		*dat|=I2C_MOIS_IN;
		I2C_SCL_OUT_L;
	}
}

