#ifndef _INA226_IIC_H
#define _INA226_IIC_H
 
#include "config.h"
/* 定义I2C的SCL连接的GPIO端口 */
#define INA226_SCL_GPIO_ID					GPIO_PB		/* GPIO端口 */
#define INA226_SCL_PIN						GPIO_PIN_7			/* 连接到SCL时钟线的GPIO */
#define INA226_SCL_AFIO_MODE				AFIO_FUN_GPIO
#define INA226_SCL_PORT					HT_GPIOB
 
/* 定义I2C的SDA连接的GPIO端口 */
#define INA226_SDA_GPIO_ID					GPIO_PB				/* GPIO端口 */
#define INA226_SDA_PIN						GPIO_PIN_8				/* 连接到SCL时钟线的GPIO */
#define INA226_SDA_AFIO_MODE				AFIO_FUN_GPIO
#define INA226_SDA_PORT					HT_GPIOB
 
#define INA226_SCL_1()						GPIO_SetOutBits(INA226_SCL_PORT, INA226_SCL_PIN)
#define INA226_SCL_0()						GPIO_ClearOutBits(INA226_SCL_PORT, INA226_SCL_PIN)
	
#define INA226_SDA_1()						GPIO_SetOutBits(INA226_SDA_PORT, INA226_SDA_PIN)
#define INA226_SDA_0()						GPIO_ClearOutBits(INA226_SDA_PORT, INA226_SDA_PIN)
#define INA226_SDA_Direction_Input()		GPIO_DirectionConfig(INA226_SDA_PORT, INA226_SDA_PIN, GPIO_DIR_IN)
#define INA226_SDA_InputConfig()			GPIO_InputConfig(INA226_SDA_PORT, INA226_SDA_PIN, ENABLE)
#define INA226_SDA_InputDisable()			GPIO_InputConfig(INA226_SDA_PORT, INA226_SDA_PIN, DISABLE)
#define INA226_SDA_Direction_Output()		GPIO_DirectionConfig(INA226_SDA_PORT, INA226_SDA_PIN, GPIO_DIR_OUT)
#define INA226_SDA_READ()					GPIO_ReadInBit(INA226_SDA_PORT, INA226_SDA_PIN)
 
#define WAITMAX							2000	
#define DELAYTIME 						2
 
extern void INA226_I2C_Initia(void);
extern void INA226_I2C_Start(void);
extern void INA226_I2C_Stop(void);
extern u8 INA226_I2C_WaitAck(void);
extern void INA226_I2C_SendByte(u8 _ucByte);
extern u8 INA226_I2C_ReadByte(void);
extern void INA226_I2C_Ack(void);
extern void INA226_I2C_NAck(void);
//FlagStatus I2C_CheckOK(void);
extern u8 INA226_I2C_CheckDevice(u8 _Address);
 uint16_t INA226_ReadData(uint8_t reg);
 
#endif
