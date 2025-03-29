#ifndef _IIC_H
#define _IIC_H
 
#include "ht32f5xxxx_01.h"
/* ����I2C��SCL���ӵ�GPIO�˿� */
#define I2C_SCL_GPIO_ID					GPIO_PB					/* GPIO�˿� */
#define I2C_SCL_PIN						GPIO_PIN_0				/* ���ӵ�SCLʱ���ߵ�GPIO */
#define I2C_SCL_AFIO_MODE				AFIO_FUN_GPIO
#define I2C_SCL_PORT					HT_GPIOB
 
/* ����I2C��SDA���ӵ�GPIO�˿� */
#define I2C_SDA_GPIO_ID					GPIO_PB					/* GPIO�˿� */
#define I2C_SDA_PIN						GPIO_PIN_1				/* ���ӵ�SCLʱ���ߵ�GPIO */
#define I2C_SDA_AFIO_MODE				AFIO_FUN_GPIO
#define I2C_SDA_PORT					HT_GPIOB
 
#define I2C_SCL_1()						GPIO_SetOutBits(I2C_SCL_PORT, I2C_SCL_PIN)
#define I2C_SCL_0()						GPIO_ClearOutBits(I2C_SCL_PORT, I2C_SCL_PIN)
	
#define I2C_SDA_1()						GPIO_SetOutBits(I2C_SDA_PORT, I2C_SDA_PIN)
#define I2C_SDA_0()						GPIO_ClearOutBits(I2C_SDA_PORT, I2C_SDA_PIN)
#define I2C_SDA_Direction_Input()		GPIO_DirectionConfig(I2C_SDA_PORT, I2C_SDA_PIN, GPIO_DIR_IN)
#define I2C_SDA_InputConfig()			GPIO_InputConfig(I2C_SDA_PORT, I2C_SDA_PIN, ENABLE)
#define I2C_SDA_InputDisable()			GPIO_InputConfig(I2C_SDA_PORT, I2C_SDA_PIN, DISABLE)
#define I2C_SDA_Direction_Output()		GPIO_DirectionConfig(I2C_SDA_PORT, I2C_SDA_PIN, GPIO_DIR_OUT)
#define I2C_SDA_READ()					GPIO_ReadInBit(I2C_SDA_PORT, I2C_SDA_PIN)
 
#define WAITMAX							2000	
#define DELAYTIME 						2
 
void I2C_Initia(void);
void I2C_Start(void);
void I2C_Stop(void);
u8 I2C_WaitAck(void);
void I2C_SendByte(u8 _ucByte);
u8 I2C_ReadByte(void);
void I2C_Ack(void);
void I2C_NAck(void);
//FlagStatus I2C_CheckOK(void);
u8 I2C_CheckDevice(u8 _Address);
 

#endif
