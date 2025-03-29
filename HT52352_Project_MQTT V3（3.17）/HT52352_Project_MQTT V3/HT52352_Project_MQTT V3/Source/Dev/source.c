#include "source.h"
#include "config.h"

int SourceWriteByte(unsigned char val)
{	
		int ret = 0;
    I2C_Start();
    I2C_SendByte(MCP4017_WRITE_ADDR);
    ret =  I2C_WaitAck();
    if(ret != 0)
    {
        I2C_Stop();
        return -1;
    }
    I2C_SendByte(val);
    ret = I2C_WaitAck();
    if(ret != 0)
    {
         I2C_Stop();
        return -1;
    }
     I2C_Stop();  
    return 0;
}
int SourceReadByte(unsigned char *val)
{
	int ret = 0;
		I2C_Start();
    I2C_SendByte(MCP4017_READ_ADDR);
    ret = I2C_WaitAck();
    if(ret != 0)
    {
        I2C_Stop();
        return -1;
    }
    *val = I2C_ReadByte();
    I2C_Stop();
    return 0;
}
