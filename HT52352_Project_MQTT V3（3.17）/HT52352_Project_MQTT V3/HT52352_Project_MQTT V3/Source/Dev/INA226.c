#include "INA226.h"
#include "config.h"
void INA226_Init(void)
{	
	INA226_I2C_Initia();
	u8 i=10;
	while(i--);
	//0100 010 100 100 111设置转换时间1.1ms,求平均值次数16，分流和总线连续模式
	INA226_SendData(CFG_REG,0x4527);
	INA226_SendData(CAL_REG,0x2800);//设置分流电压转电流转换参数 以最大测1A来设置分辨率
}


void INA226_SendData(uint8_t reg,uint16_t data)
{
	uint8_t temp=0;
	INA226_I2C_Start();//IIC开始
	INA226_I2C_SendByte(INA226_ADDR1); //这里我就直接把从机的写地址写入函数了（也就是0x80），没作为传参参数
	INA226_I2C_WaitAck(); //接收应答
	
	INA226_I2C_SendByte(reg);//发送寄存器地址，表明我们对INA226的这个寄存器进行写入
	INA226_I2C_WaitAck();//接收应答
	
	temp = (uint8_t)(data>>8); 
	INA226_I2C_SendByte(temp);//写入高8位
	INA226_I2C_WaitAck();//接收应答
	
	temp = (uint8_t)(data&0x00FF);
	INA226_I2C_SendByte(temp); //写入低8位
	INA226_I2C_WaitAck();//接收应答
	
	INA226_I2C_Stop();//IIC结束
}
uint16_t  INA226_GetID(void)
{
	uint16_t temp=0;
	temp = INA226_ReadData(INA226_GET_ADDR);
	return temp;
}
