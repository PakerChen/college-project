#include "bsp_spi.h"
#define  SPI1_SCLK_PIN 		  (GPIO_PIN_2)
#define  SPI1_MOSI_PIN 			(GPIO_PIN_3)
#define  SPI1_MISO_PIN			(GPIO_PIN_6)

#define	SPI_MOIS_OUT_H HT_GPIOC->SRR = GPIO_PIN_3
#define	SPI_MOIS_OUT_L HT_GPIOC->RR = GPIO_PIN_3

#define	I2C_MISO_IN  GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_6)

#define	SPI_SCLK_OUT_H HT_GPIOC->SRR = GPIO_PIN_2
#define	SPI_SCLK_OUT_L HT_GPIOC->RR = GPIO_PIN_2

//从机选择线
#define  SPI1_SEL_GPIO_IC    (GPIO_PC)
#define  SPI1_SEL_PIN 			(GPIO_PIN_1)

/*******************************************************************************************************
** 函数: SPI_Del, SPI延时函数
**------------------------------------------------------------------------------------------------------
** 参数: NONE 
** 返回: NONE
*******************************************************************************************************/
void SPI_Del(unsigned long ms)
{
	while(ms--);
}


/*******************************************************************************************************
** 函数: SPI_Init, SPI延时函数
**------------------------------------------------------------------------------------------------------
** 参数: NONE 
** 返回: NONE
*******************************************************************************************************/
void SPI1_Init(void)
{
   CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
    CKCUClock.Bit.PC         = 1;
	  CKCUClock.Bit.PB        = 1;
    CKCUClock.Bit.AFIO       = 1;
    CKCUClock.Bit.BKP        = 1;
    CKCU_PeripClockConfig(CKCUClock, ENABLE);
	if (PWRCU_CheckReadyAccessed() != PWRCU_OK)
	{
		while (1);
	}
	AFIO_GPxConfig(GPIO_PC, SPI1_SCLK_PIN, AFIO_MODE_DEFAULT);//config SCLK
	GPIO_DirectionConfig(HT_GPIOC, SPI1_SCLK_PIN, GPIO_DIR_OUT);	 
	
	AFIO_GPxConfig(GPIO_PC, SPI1_MOSI_PIN, AFIO_MODE_DEFAULT);//config MOSI
	GPIO_DirectionConfig(HT_GPIOC, SPI1_MOSI_PIN, GPIO_DIR_OUT);  
	
	AFIO_GPxConfig(GPIO_PB, SPI1_MISO_PIN, AFIO_MODE_DEFAULT);//config MISO
	GPIO_DirectionConfig(HT_GPIOB, SPI1_MISO_PIN, GPIO_DIR_IN);  
	GPIO_InputConfig(HT_GPIOB,SPI1_MISO_PIN,ENABLE);  
//    /*  Configure SPI SEL pin (PC1)*/
//    GPIO_SetOutBits(GPIO_PORT[SPI1_SEL_GPIO_IC], SPI1_SEL_PIN);
//    GPIO_DirectionConfig(GPIO_PORT[SPI1_SEL_GPIO_IC], SPI1_SEL_PIN, GPIO_DIR_OUT);
	 	AFIO_GPxConfig(GPIO_PC, SPI1_SEL_PIN, AFIO_MODE_DEFAULT);//config MOSI
	GPIO_DirectionConfig(HT_GPIOC, SPI1_SEL_PIN, GPIO_DIR_OUT);  

}



/*******************************************************************************************************
** 函数: SPI2_ReadWrite, SPI发送一个字节，同时接收一个字节
**------------------------------------------------------------------------------------------------------
** 参数: dat 发送数据 
** 返回: 接收数据
*******************************************************************************************************/
unsigned char SPI1_ReadWrite(unsigned char  dat)
{
	unsigned char i,temp=0 ;											 
	for(i=0;i<8;i++)
	{
		if((  dat<<i )& 0x80)SPI_MOIS_OUT_H;
		else SPI_MOIS_OUT_L;
		SPI_SCLK_OUT_H;		
		SPI_Del(0x10);
		temp=( temp<<1 )| I2C_MISO_IN;
		SPI_SCLK_OUT_L;		 
		SPI_Del(0x10);
	}
	return temp;
}
