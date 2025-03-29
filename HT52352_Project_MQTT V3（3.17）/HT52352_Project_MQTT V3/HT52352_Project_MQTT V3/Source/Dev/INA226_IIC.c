#include "INA226_IIC.h"
#include "config.h"
 
static void INA226_I2C_CKCU_Config()
{
	CKCU_PeripClockConfig_TypeDef CCLOCK;
	
	CCLOCK.Bit.PB   = 1;
	CCLOCK.Bit.AFIO  = 1;
	
	CKCU_PeripClockConfig(CCLOCK, ENABLE);
}
 
static void INA226_I2C_GPIO_Config()
{
	AFIO_GPxConfig(INA226_SCL_GPIO_ID, INA226_SCL_PIN, INA226_SCL_AFIO_MODE);
	AFIO_GPxConfig(INA226_SDA_GPIO_ID, INA226_SDA_PIN, INA226_SDA_AFIO_MODE);
	
	/* Configure the GPIO pin                                                                                 */
	GPIO_PullResistorConfig(INA226_SCL_PORT, INA226_SCL_PIN, GPIO_PR_DISABLE);
	GPIO_DriveConfig(INA226_SCL_PORT, INA226_SCL_PIN, GPIO_DV_8MA);
	GPIO_DirectionConfig(INA226_SCL_PORT, INA226_SCL_PIN, GPIO_DIR_OUT);
	GPIO_OpenDrainConfig(INA226_SCL_PORT,INA226_SCL_PIN,ENABLE);
	
	GPIO_PullResistorConfig(INA226_SDA_PORT, INA226_SDA_PIN, GPIO_PR_DISABLE);
	GPIO_DriveConfig(INA226_SDA_PORT, INA226_SDA_PIN, GPIO_DV_8MA);
	GPIO_DirectionConfig(INA226_SDA_PORT, INA226_SDA_PIN, GPIO_DIR_OUT);
	GPIO_OpenDrainConfig(INA226_SDA_PORT,INA226_SDA_PIN,ENABLE);
}
 
void INA226_I2C_Initia()
{
	INA226_I2C_CKCU_Config();
	INA226_I2C_GPIO_Config();
}
 
void INA226_I2C_Start()
{
	INA226_SDA_Direction_Output();
	INA226_SCL_1();
	INA226_SDA_1();
	SysTickDelayUs(DELAYTIME);
	
	INA226_SDA_0();
	SysTickDelayUs(DELAYTIME);
	
	INA226_SCL_0();
	SysTickDelayUs(DELAYTIME);
}
 
void INA226_I2C_Stop()
{
	INA226_SDA_Direction_Output();
	INA226_SDA_0();
	INA226_SCL_1();
	SysTickDelayUs(DELAYTIME);
	INA226_SDA_1();
}
 
/* 返回0表示正确应答，1表示无器件响应 */
u8 INA226_I2C_WaitAck()
{
	u8 ack;
	
	INA226_SCL_1();
	SysTickDelayUs(DELAYTIME);
	
	INA226_SDA_1();
	SysTickDelayUs(DELAYTIME);
	INA226_SDA_Direction_Input();
	INA226_SDA_InputConfig();
	
	if( INA226_SDA_READ() ) {
		ack = 1;
	}
	else {
		ack = 0;
	}
	INA226_SCL_0();
	SysTickDelayUs(DELAYTIME);
	return ack;
}
 
void INA226_I2C_SendByte(u8 _ucByte)
{
	u8 i;
	INA226_SDA_Direction_Output();
	/* 先发送字节的高位bit7 */
	for (i = 0; i < 8; i++)
	{		
		if (_ucByte & 0x80)
		{
			INA226_SDA_1();
		}
		else
		{
			INA226_SDA_0();
		}
		
		SysTickDelayUs(DELAYTIME);
		INA226_SCL_1();
		SysTickDelayUs(DELAYTIME);
		INA226_SCL_0();
		
		if (i == 7)
		{
			 INA226_SDA_1(); // 释放总线
		}
		_ucByte <<= 1;	/* 左移一个bit */
		SysTickDelayUs(DELAYTIME);
	}
}
 
u8 INA226_I2C_ReadByte()
{
	
	u8 i;
	u8 data;
	
	data = 0;
	INA226_SDA_Direction_Input();
	INA226_SDA_InputConfig();
	
	for(i = 0;i < 8;i++)
	{
		data <<= 1;
		INA226_SCL_1();
		SysTickDelayUs(DELAYTIME);
		
		INA226_SDA_Direction_Input();
		INA226_SDA_InputConfig();
	
		if( INA226_SDA_READ() ) {
			data++;
		}
		INA226_SCL_0();
		SysTickDelayUs(DELAYTIME);
	}
	return data;
}
 uint16_t INA226_ReadData(uint8_t reg)
{
	uint16_t temp=0;
	
	INA226_I2C_Start();//IIC开始
	INA226_I2C_SendByte(INA226_ADDR1); //发送从机写地址
	INA226_I2C_WaitAck();//接收从机应答
	
	INA226_I2C_SendByte(reg);//发送寄存器地址
	INA226_I2C_WaitAck();//接收从机应答
	
	INA226_I2C_Stop();//IIC停止
	
	INA226_I2C_Start();//IIC开始
	
	INA226_I2C_SendByte(INA226_ADDR1+1);//从机写地址+1 = 从机读地址
	INA226_I2C_WaitAck();//接收从机应答
	
	temp = INA226_I2C_ReadByte();//接收高8位数据到temp
	INA226_I2C_Ack();//0表示发送应答给从机，让从机不要再发数据了，我先把这16位接收完
	temp = temp <<8;//左移8位，让出低位
	temp = temp | INA226_I2C_ReadByte();//接收低8位数据到temp
	//MyI2C_SendAck(1);//1表示发送应答
 
	INA226_I2C_Stop();//IIC停止
	return temp; 
}


void INA226_I2C_Ack()
{
	INA226_SDA_Direction_Output();
	INA226_SDA_0();	/* CPU驱动SDA = 0 */
	SysTickDelayUs(DELAYTIME);
	
	INA226_SCL_1();	/* CPU产生1个时钟 */
	SysTickDelayUs(DELAYTIME);
	
	INA226_SCL_0();
	SysTickDelayUs(DELAYTIME);
	INA226_SDA_1();	/* CPU释放SDA总线 */
}
 
void INA226_I2C_NAck()
{
	INA226_SDA_Direction_Output();
	INA226_SDA_1();	/* CPU驱动SDA = 1 */
	SysTickDelayUs(DELAYTIME);
	
	INA226_SCL_1();	/* CPU产生1个时钟 */
	SysTickDelayUs(DELAYTIME);
	
	INA226_SCL_0();
	SysTickDelayUs(DELAYTIME);
}
