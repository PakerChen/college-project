#ifndef _SOURCEIIC_H
#define _SOURCEIIC_H
 
#include "ht32f5xxxx_01.h"
 
#define EEPROM_I2C_WR				0		/* 写控制bit */
#define EEPROM_I2C_RD				1		/* 读控制bit */
#define EEPROM_DEV_ADDR				0xA0	/* 24xx02的设备地址 */
#define EEPROM_PAGE_SIZE		  	8		/* 24xx02的页面大小 */
#define EEPROM_SIZE				  	256		/* 24xx02总容量 */
 
/* 定义I2C的SCL连接的GPIO端口 */
#define I2C_SCL_GPIO_ID					GPIO_PA					/* GPIO端口 */
#define I2C_SCL_PIN						GPIO_PIN_0				/* 连接到SCL时钟线的GPIO */
#define I2C_SCL_AFIO_MODE				AFIO_FUN_GPIO
#define I2C_SCL_PORT					HT_GPIOA
 
/* 定义I2C的SDA连接的GPIO端口 */
#define I2C_SDA_GPIO_ID					GPIO_PA			/* GPIO端口 */
#define I2C_SDA_PIN						GPIO_PIN_1			/* 连接到SCL时钟线的GPIO */
#define I2C_SDA_AFIO_MODE				AFIO_FUN_GPIO
#define I2C_SDA_PORT					HT_GPIOA
 
#define I2C_SCL_1()						GPIO_SetOutBits(I2C_SCL_PORT, I2C_SCL_PIN)
#define I2C_SCL_0()						GPIO_ClearOutBits(I2C_SCL_PORT, I2C_SCL_PIN)
	
#define I2C_SDA_1()						GPIO_SetOutBits(I2C_SDA_PORT, I2C_SDA_PIN)
#define I2C_SDA_0()						GPIO_ClearOutBits(I2C_SDA_PORT, I2C_SDA_PIN)

#define I2C_SDA_Direction_Input()		GPIO_DirectionConfig(I2C_SDA_PORT, I2C_SDA_PIN, GPIO_DIR_IN)

#define I2C_SDA_InputConfig()			GPIO_InputConfig(I2C_SDA_PORT, I2C_SDA_PIN, ENABLE)
#define I2C_SDA_InputDisable()			GPIO_InputConfig(I2C_SDA_PORT, I2C_SDA_PIN, DISABLE)

#define I2C_SDA_Direction_Output()		GPIO_DirectionConfig(I2C_SDA_PORT, I2C_SDA_PIN, GPIO_DIR_OUT)
#define I2C_SDA_READ()					GPIO_ReadInBit(I2C_SDA_PORT, I2C_SDA_PIN)
#define Systick_Delay_us SysTickDelayUs

#define WAITMAX							2000	
#define DELAYTIME 						2
 
void I2C_Initia(void);
void I2C_Start(void);
void I2C_Stop(void);
u8 I2C_WaitAck(void);
void I2C_SendByte(u8 _ucByte);
u8 I2C_ReadByte(unsigned char ack);
void I2C_Ack(void);
void I2C_NAck(void);
//FlagStatus I2C_CheckOK(void);
u8 I2C_CheckDevice(u8 _Address);
 
u8 EE_CheckOK(void);
u8 EE_WriteBytes(u8 *_pWriteBuf, u16 _usAddress, u16 _usSize);
u8 EE_ReadBytes(u8 *_pReadBuf, u16 _usAddress, u16 _usSize);
#endif
