#include "bsp_spi.h"
#define  SPI1_SCLK_PIN 		  (GPIO_PIN_2)
#define  SPI1_MOSI_PIN 			(GPIO_PIN_3)
#define  SPI1_MISO_PIN			(GPIO_PIN_6)

#define	SPI_MOIS_OUT_H HT_GPIOC->SRR = GPIO_PIN_3
#define	SPI_MOIS_OUT_L HT_GPIOC->RR = GPIO_PIN_3

#define	I2C_MISO_IN  GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_6)

#define	SPI_SCLK_OUT_H HT_GPIOC->SRR = GPIO_PIN_2
#define	SPI_SCLK_OUT_L HT_GPIOC->RR = GPIO_PIN_2

//�ӻ�ѡ����
#define  SPI1_SEL_GPIO_IC    (GPIO_PC)
#define  SPI1_SEL_PIN 			(GPIO_PIN_1)

/*******************************************************************************************************
** ����: SPI_Del, SPI��ʱ����
**------------------------------------------------------------------------------------------------------
** ����: NONE 
** ����: NONE
*******************************************************************************************************/
void SPI_Del(unsigned long ms)
{
	while(ms--);
}


/*******************************************************************************************************
** ����: SPI_Init, SPI��ʱ����
**------------------------------------------------------------------------------------------------------
** ����: NONE 
** ����: NONE
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
** ����: SPI2_ReadWrite, SPI����һ���ֽڣ�ͬʱ����һ���ֽ�
**------------------------------------------------------------------------------------------------------
** ����: dat �������� 
** ����: ��������
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
