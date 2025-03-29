#include "config.h"
#include "sourceIIC.h"
static void I2C_CKCU_Config()
{
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
    CKCUClock.Bit.PA         = 1;
    CKCUClock.Bit.AFIO       = 1;
    CKCU_PeripClockConfig(CKCUClock, ENABLE);
}
 
static void I2C_GPIO_Config()
{
	AFIO_GPxConfig(I2C_SCL_GPIO_ID, I2C_SCL_PIN, AFIO_MODE_DEFAULT);
	AFIO_GPxConfig(I2C_SDA_GPIO_ID, I2C_SDA_PIN, AFIO_MODE_DEFAULT);
	
	/* Configure the GPIO pin                                                                                 */
	GPIO_PullResistorConfig(I2C_SCL_PORT, I2C_SCL_PIN, GPIO_PR_DISABLE);
	GPIO_DriveConfig(I2C_SCL_PORT, I2C_SCL_PIN, GPIO_DV_8MA);
	GPIO_DirectionConfig(I2C_SCL_PORT, I2C_SCL_PIN, GPIO_DIR_OUT);
	
	GPIO_PullResistorConfig(I2C_SDA_PORT, I2C_SDA_PIN, GPIO_PR_DISABLE);
	GPIO_DriveConfig(I2C_SDA_PORT, I2C_SDA_PIN, GPIO_DV_8MA);
	GPIO_DirectionConfig(I2C_SDA_PORT, I2C_SDA_PIN, GPIO_DIR_OUT);
	
	GPIO_OpenDrainConfig(I2C_SCL_PORT, I2C_SCL_PIN, ENABLE);
	
	GPIO_OpenDrainConfig(I2C_SDA_PORT, I2C_SDA_PIN, ENABLE);
}
 
void I2C_Initia()
{
	I2C_CKCU_Config();
	I2C_GPIO_Config();
	
	/* 一定要先发一遍停止信号 */
	I2C_Stop();
}
 
void I2C_Start()
{
	I2C_SDA_Direction_Output();	
	I2C_SCL_1();
	I2C_SDA_1();
	SysTickDelayUs(2);
	I2C_SDA_0();
	SysTickDelayUs(2);
	I2C_SCL_0();
}
 
void I2C_Stop()
{
	I2C_SDA_Direction_Output();
	I2C_SCL_0();
	I2C_SDA_0();
	Systick_Delay_us(2);
	I2C_SCL_1();
	I2C_SDA_1();
	Systick_Delay_us(2);
}
 
/* 返回0表示正确应答，1表示无器件响应 */
u8 I2C_WaitAck()
{
	u8 ucErrTime=0;
	I2C_SDA_Direction_Input();
	I2C_SDA_1();Systick_Delay_us(2);
	I2C_SCL_1();Systick_Delay_us(2);
	while(I2C_SDA_READ())
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			I2C_Stop();
			return 1;
		}
	}
	I2C_SCL_0();
	return 0;
}
 
void I2C_SendByte(u8 _ucByte)
{
	u8 i;
	/* 先发送字节的高位bit7 */
	I2C_SDA_Direction_Output();
	I2C_SCL_0();
	for (i = 0; i < 8; i++)
	{		
		if ((_ucByte & 0x80)>>7)
		{
			I2C_SDA_1();
		}
		else
		{
			I2C_SDA_0();
		}
		_ucByte<<=1;
		Systick_Delay_us(2);
		I2C_SCL_1();
		Systick_Delay_us(2);
		I2C_SCL_0();
		Systick_Delay_us(2);
	}
}
 
u8 I2C_ReadByte(unsigned char ack)
{
	u8 i,receive=0;
	I2C_SDA_Direction_Input();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
				I2C_SCL_0();		
        SysTickDelayUs(2);
				I2C_SCL_1();
        receive<<=1;
        if(I2C_SDA_READ())receive++;   
		SysTickDelayUs(1); 
    }					 
    if (!ack)
        I2C_NAck();//发送nACK
    else
        I2C_Ack(); //发送ACK   
    return receive;
}
 
// 返回值为0表示正确，返回1表示未探测到
u8 I2C_CheckDevice(u8 _Address)
{
//	u8 time = 0;
	u8 ucAck = 0;
	
	I2C_Start();
	I2C_SendByte(_Address | EEPROM_I2C_WR);
	ucAck =I2C_WaitAck();
	I2C_SDA_Direction_Output();
	I2C_Stop();
 
	return ucAck;
}
 
void I2C_Ack()
{
	I2C_SCL_0();
	I2C_SDA_Direction_Output();
	I2C_SDA_0();
	SysTickDelayUs(2);
	I2C_SCL_1();
	SysTickDelayUs(2);
	I2C_SCL_0();
}
 
void I2C_NAck()
{
	I2C_SCL_0();
	I2C_SDA_Direction_Output();
	I2C_SDA_1();
	SysTickDelayUs(2);
	I2C_SCL_1();
	SysTickDelayUs(2);
	I2C_SCL_0();
}
