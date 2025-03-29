#include "config.h"   // Device header
#include "MySPI.h"

/*�������ò�*/

/**
  * ��    ����SPIдSS���ŵ�ƽ
  * ��    ����BitValue Э��㴫��ĵ�ǰ��Ҫд��SS�ĵ�ƽ����Χ0~1
  * �� �� ֵ����
  * ע������˺�����Ҫ�û�ʵ�����ݣ���BitValueΪ0ʱ����Ҫ��SSΪ�͵�ƽ����BitValueΪ1ʱ����Ҫ��SSΪ�ߵ�ƽ
  */
void MySPI_W_SS(uint8_t BitValue)
{
	GPIO_WriteOutBits(HT_GPIOA,GPIO_PIN_6,(FlagStatus)BitValue);  //����BitValue������SS���ŵĵ�ƽ
}


/**
  * ��    ����SPIдSCK���ŵ�ƽ
  * ��    ����BitValue Э��㴫��ĵ�ǰ��Ҫд��SCK�ĵ�ƽ����Χ0~1
  * �� �� ֵ����
  * ע������˺�����Ҫ�û�ʵ�����ݣ���BitValueΪ0ʱ����Ҫ��SCKΪ�͵�ƽ����BitValueΪ1ʱ����Ҫ��SCKΪ�ߵ�ƽ
  */

void MySPI_W_SCK(uint8_t BitValue)
{
	GPIO_WriteOutBits(HT_GPIOC,GPIO_PIN_2,(FlagStatus)BitValue);  //����BitValue������SCK���ŵĵ�ƽ
}
/**
  * ��    ����SPIдMOSI���ŵ�ƽ
  * ��    ����BitValue Э��㴫��ĵ�ǰ��Ҫд��MOSI�ĵ�ƽ����Χ0~0xFF
  * �� �� ֵ����
  * ע������˺�����Ҫ�û�ʵ�����ݣ���BitValueΪ0ʱ����Ҫ��MOSIΪ�͵�ƽ����BitValue��0ʱ����Ҫ��MOSIΪ�ߵ�ƽ
  */
void MySPI_W_MOSI(uint8_t BitValue)
{
	GPIO_WriteOutBits(HT_GPIOC,GPIO_PIN_3,(FlagStatus)BitValue);  //����BitValue������MOSI���ŵĵ�ƽ��BitValueҪʵ�ַ�0��1������
}
/**
  * ��    ����I2C��MISO���ŵ�ƽ
  * ��    ������
  * �� �� ֵ��Э�����Ҫ�õ��ĵ�ǰMISO�ĵ�ƽ����Χ0~1
  * ע������˺�����Ҫ�û�ʵ�����ݣ���ǰMISOΪ�͵�ƽʱ������0����ǰMISOΪ�ߵ�ƽʱ������1
  */
uint8_t MySPI_R_MISO(void)
{
	return GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_6);  //��ȡMISO��ƽ������
}
/**
  * ��    ����SPI��ʼ��
  * ��    ������
  * �� �� ֵ����
  * ע������˺�����Ҫ�û�ʵ�����ݣ�ʵ��SS��SCK��MOSI��MISO���ŵĳ�ʼ��
  */
void MySPI_init(void)
{
	/*����ʱ��*/
		CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
    CKCUClock.Bit.PB         = 1;			//����GPIOB��ʱ��
	  CKCUClock.Bit.PA         = 1;		//����GPIOA��ʱ��
		CKCUClock.Bit.PC         = 1;		//����GPIOC��ʱ��
		CKCUClock.Bit.AFIO         = 1;
		CKCU_PeripClockConfig(CKCUClock, ENABLE);
	 
	 AFIO_GPxConfig(GPIO_PA, GPIO_PIN_6, AFIO_MODE_DEFAULT);//config CS
	 GPIO_DirectionConfig(HT_GPIOA,GPIO_PIN_6,GPIO_DIR_OUT);
	 
	 AFIO_GPxConfig(GPIO_PC, GPIO_PIN_2, AFIO_MODE_DEFAULT);//config SCLK
	 GPIO_DirectionConfig(HT_GPIOC,GPIO_PIN_2,GPIO_DIR_OUT);
	 
	 AFIO_GPxConfig(GPIO_PC, GPIO_PIN_3, AFIO_MODE_DEFAULT);//config MOSI
	 GPIO_DirectionConfig(HT_GPIOC,GPIO_PIN_3,GPIO_DIR_OUT);
	 
	 AFIO_GPxConfig(GPIO_PB, GPIO_PIN_6, AFIO_MODE_DEFAULT);//config MISO
	 GPIO_DirectionConfig(HT_GPIOB,GPIO_PIN_6,GPIO_DIR_IN);
	 GPIO_InputConfig(HT_GPIOB,GPIO_PIN_6,ENABLE);
	 
	 /*����Ĭ�ϵ�ƽ*/
	 MySPI_W_SS(1);      //SSĬ�ϸߵ�ƽ
	 MySPI_W_SCK(0);		 //SCKĬ�ϵ͵�ƽ
}
/*Э���*/

/**
  * ��    ����SPI��ʼ
  * ��    ������
  * �� �� ֵ����
  */
void MySPI_Start(void)
{
	 MySPI_W_SS(0);			//����SS����ʼʱ��
}
/**
  * ��    ����SPI��ֹ
  * ��    ������
  * �� �� ֵ����
  */
void MySPI_Stop(void)
{
	 MySPI_W_SS(1);		//����SS����ֹʱ��
}
/**
  * ��    ����SPI��������һ���ֽڣ�ʹ��SPIģʽ0
  * ��    ����ByteSend Ҫ���͵�һ���ֽ�
  * �� �� ֵ�����յ�һ���ֽ�
  */
uint8_t MySPI_SwapByte(uint8_t ByteSend)
{
	uint8_t i,ByteReceive=0x00; 		//������յ����ݣ�������ֵ0x00���˴����븳��ֵ0x00��������õ�
	
	for(i=0;i<8;i++)		//ѭ��8�Σ����ν���ÿһλ����
	{
		MySPI_W_MOSI(ByteSend&(0x80>>i)); //ʹ������ķ�ʽȡ��ByteSend��ָ��һλ���ݲ�д�뵽MOSI��
		 MySPI_W_SCK(1);			//����SCK���������Ƴ�����
		if(MySPI_R_MISO()==1)
		{
			ByteReceive|=(0x80>>i);		//��ȡMISO���ݣ����洢��Byte����
		}														//��MISOΪ1ʱ���ñ���ָ��λΪ1����MISOΪ0ʱ����������ָ��λΪĬ�ϵĳ�ֵ0
		MySPI_W_SCK(0);								//����SCK���½�����������
	}
	return ByteReceive;						//���ؽ��յ���һ���ֽ�����
}
