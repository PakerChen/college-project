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
 
/* ����0��ʾ��ȷӦ��1��ʾ��������Ӧ */
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
	/* �ȷ����ֽڵĸ�λbit7 */
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
			 INA226_SDA_1(); // �ͷ�����
		}
		_ucByte <<= 1;	/* ����һ��bit */
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
	
	INA226_I2C_Start();//IIC��ʼ
	INA226_I2C_SendByte(INA226_ADDR1); //���ʹӻ�д��ַ
	INA226_I2C_WaitAck();//���մӻ�Ӧ��
	
	INA226_I2C_SendByte(reg);//���ͼĴ�����ַ
	INA226_I2C_WaitAck();//���մӻ�Ӧ��
	
	INA226_I2C_Stop();//IICֹͣ
	
	INA226_I2C_Start();//IIC��ʼ
	
	INA226_I2C_SendByte(INA226_ADDR1+1);//�ӻ�д��ַ+1 = �ӻ�����ַ
	INA226_I2C_WaitAck();//���մӻ�Ӧ��
	
	temp = INA226_I2C_ReadByte();//���ո�8λ���ݵ�temp
	INA226_I2C_Ack();//0��ʾ����Ӧ����ӻ����ôӻ���Ҫ�ٷ������ˣ����Ȱ���16λ������
	temp = temp <<8;//����8λ���ó���λ
	temp = temp | INA226_I2C_ReadByte();//���յ�8λ���ݵ�temp
	//MyI2C_SendAck(1);//1��ʾ����Ӧ��
 
	INA226_I2C_Stop();//IICֹͣ
	return temp; 
}


void INA226_I2C_Ack()
{
	INA226_SDA_Direction_Output();
	INA226_SDA_0();	/* CPU����SDA = 0 */
	SysTickDelayUs(DELAYTIME);
	
	INA226_SCL_1();	/* CPU����1��ʱ�� */
	SysTickDelayUs(DELAYTIME);
	
	INA226_SCL_0();
	SysTickDelayUs(DELAYTIME);
	INA226_SDA_1();	/* CPU�ͷ�SDA���� */
}
 
void INA226_I2C_NAck()
{
	INA226_SDA_Direction_Output();
	INA226_SDA_1();	/* CPU����SDA = 1 */
	SysTickDelayUs(DELAYTIME);
	
	INA226_SCL_1();	/* CPU����1��ʱ�� */
	SysTickDelayUs(DELAYTIME);
	
	INA226_SCL_0();
	SysTickDelayUs(DELAYTIME);
}
