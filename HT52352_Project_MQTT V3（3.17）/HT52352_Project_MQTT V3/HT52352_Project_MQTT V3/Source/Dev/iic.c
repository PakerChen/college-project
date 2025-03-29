#include "iic.h"
#include "config.h"
 
static void I2C_CKCU_Config()
{
	CKCU_PeripClockConfig_TypeDef CCLOCK;
	
	CCLOCK.Bit.PB     = 1;
	CCLOCK.Bit.AFIO  = 1;
	
	CKCU_PeripClockConfig(CCLOCK, ENABLE);
}
 
static void I2C_GPIO_Config()
{
	AFIO_GPxConfig(I2C_SCL_GPIO_ID, I2C_SCL_PIN, I2C_SCL_AFIO_MODE);
	AFIO_GPxConfig(I2C_SDA_GPIO_ID, I2C_SDA_PIN, I2C_SDA_AFIO_MODE);
	
	/* Configure the GPIO pin                                                                                 */
	GPIO_PullResistorConfig(I2C_SCL_PORT, I2C_SCL_PIN, GPIO_PR_DISABLE);
	GPIO_DriveConfig(I2C_SCL_PORT, I2C_SCL_PIN, GPIO_DV_8MA);
	GPIO_DirectionConfig(I2C_SCL_PORT, I2C_SCL_PIN, GPIO_DIR_OUT);
	GPIO_OpenDrainConfig(I2C_SCL_PORT,I2C_SCL_PIN,ENABLE);
	
	GPIO_PullResistorConfig(I2C_SDA_PORT, I2C_SDA_PIN, GPIO_PR_DISABLE);
	GPIO_DriveConfig(I2C_SDA_PORT, I2C_SDA_PIN, GPIO_DV_8MA);
	GPIO_DirectionConfig(I2C_SDA_PORT, I2C_SDA_PIN, GPIO_DIR_OUT);
	GPIO_OpenDrainConfig(I2C_SDA_PORT,I2C_SDA_PIN,ENABLE);
}
 
void I2C_Initia()
{
	I2C_CKCU_Config();
	I2C_GPIO_Config();
}
 
void I2C_Start()
{
	I2C_SDA_Direction_Output();
	I2C_SCL_1();
	I2C_SDA_1();
	SysTickDelayUs(DELAYTIME);
	
	I2C_SDA_0();
	SysTickDelayUs(DELAYTIME);
	
	I2C_SCL_0();
	SysTickDelayUs(DELAYTIME);
}
 
void I2C_Stop()
{
	I2C_SDA_Direction_Output();
	I2C_SDA_0();
	I2C_SCL_1();
	SysTickDelayUs(DELAYTIME);
	I2C_SDA_1();
}
 
/* 返回0表示正确应答，1表示无器件响应 */
u8 I2C_WaitAck()
{
	u8 ack;
	
	I2C_SCL_1();
	SysTickDelayUs(DELAYTIME);
	
	I2C_SDA_1();
	SysTickDelayUs(DELAYTIME);
	I2C_SDA_Direction_Input();
	I2C_SDA_InputConfig();
	
	if( I2C_SDA_READ() ) {
		ack = 1;
	}
	else {
		ack = 0;
	}
	I2C_SCL_0();
	SysTickDelayUs(DELAYTIME);
	return ack;
}
 
void I2C_SendByte(u8 _ucByte)
{
	u8 i;
	I2C_SDA_Direction_Output();
	/* 先发送字节的高位bit7 */
	for (i = 0; i < 8; i++)
	{		
		if (_ucByte & 0x80)
		{
			I2C_SDA_1();
		}
		else
		{
			I2C_SDA_0();
		}
		
		SysTickDelayUs(DELAYTIME);
		I2C_SCL_1();
		SysTickDelayUs(DELAYTIME);
		I2C_SCL_0();
		
		if (i == 7)
		{
			 I2C_SDA_1(); // 释放总线
		}
		_ucByte <<= 1;	/* 左移一个bit */
		SysTickDelayUs(DELAYTIME);
	}
}
 
u8 I2C_ReadByte()
{
	
	u8 i;
	u8 data;
	
	data = 0;
	I2C_SDA_Direction_Input();
	I2C_SDA_InputConfig();
	
	for(i = 0;i < 8;i++)
	{
		data <<= 1;
		I2C_SCL_1();
		SysTickDelayUs(DELAYTIME);
		
		I2C_SDA_Direction_Input();
		I2C_SDA_InputConfig();
	
		if( I2C_SDA_READ() ) {
			data++;
		}
		I2C_SCL_0();
		SysTickDelayUs(DELAYTIME);
	}
	return data;
}
 
// 返回值为0表示正确，返回1表示未探测到
//u8 I2C_CheckDevice(u8 _Address)
//{
////	u8 time = 0;
//	u8 ucAck = 0;
//	
//	I2C_Start();
//	I2C_SendByte(_Address | EEPROM_I2C_WR);
//	ucAck =I2C_WaitAck();
//	I2C_SDA_Direction_Output();
//	I2C_Stop();
// 
//	return ucAck;
//}
 
void I2C_Ack()
{
	I2C_SDA_Direction_Output();
	I2C_SDA_0();	/* CPU驱动SDA = 0 */
	SysTickDelayUs(DELAYTIME);
	
	I2C_SCL_1();	/* CPU产生1个时钟 */
	SysTickDelayUs(DELAYTIME);
	
	I2C_SCL_0();
	SysTickDelayUs(DELAYTIME);
	I2C_SDA_1();	/* CPU释放SDA总线 */
}
 
void I2C_NAck()
{
	I2C_SDA_Direction_Output();
	I2C_SDA_1();	/* CPU驱动SDA = 1 */
	SysTickDelayUs(DELAYTIME);
	
	I2C_SCL_1();	/* CPU产生1个时钟 */
	SysTickDelayUs(DELAYTIME);
	
	I2C_SCL_0();
	SysTickDelayUs(DELAYTIME);
}

