#include "INA226.h"
#include "config.h"
void INA226_Init(void)
{	
	INA226_I2C_Initia();
	u8 i=10;
	while(i--);
	//0100 010 100 100 111����ת��ʱ��1.1ms,��ƽ��ֵ����16����������������ģʽ
	INA226_SendData(CFG_REG,0x4527);
	INA226_SendData(CAL_REG,0x2800);//���÷�����ѹת����ת������ ������1A�����÷ֱ���
}


void INA226_SendData(uint8_t reg,uint16_t data)
{
	uint8_t temp=0;
	INA226_I2C_Start();//IIC��ʼ
	INA226_I2C_SendByte(INA226_ADDR1); //�����Ҿ�ֱ�ӰѴӻ���д��ַд�뺯���ˣ�Ҳ����0x80����û��Ϊ���β���
	INA226_I2C_WaitAck(); //����Ӧ��
	
	INA226_I2C_SendByte(reg);//���ͼĴ�����ַ���������Ƕ�INA226������Ĵ�������д��
	INA226_I2C_WaitAck();//����Ӧ��
	
	temp = (uint8_t)(data>>8); 
	INA226_I2C_SendByte(temp);//д���8λ
	INA226_I2C_WaitAck();//����Ӧ��
	
	temp = (uint8_t)(data&0x00FF);
	INA226_I2C_SendByte(temp); //д���8λ
	INA226_I2C_WaitAck();//����Ӧ��
	
	INA226_I2C_Stop();//IIC����
}
uint16_t  INA226_GetID(void)
{
	uint16_t temp=0;
	temp = INA226_ReadData(INA226_GET_ADDR);
	return temp;
}
