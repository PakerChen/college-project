#include "config.h"   // Device header
#include "MySPI.h"

/*引脚配置层*/

/**
  * 函    数：SPI写SS引脚电平
  * 参    数：BitValue 协议层传入的当前需要写入SS的电平，范围0~1
  * 返 回 值：无
  * 注意事项：此函数需要用户实现内容，当BitValue为0时，需要置SS为低电平，当BitValue为1时，需要置SS为高电平
  */
void MySPI_W_SS(uint8_t BitValue)
{
	GPIO_WriteOutBits(HT_GPIOA,GPIO_PIN_6,(FlagStatus)BitValue);  //根据BitValue，设置SS引脚的电平
}


/**
  * 函    数：SPI写SCK引脚电平
  * 参    数：BitValue 协议层传入的当前需要写入SCK的电平，范围0~1
  * 返 回 值：无
  * 注意事项：此函数需要用户实现内容，当BitValue为0时，需要置SCK为低电平，当BitValue为1时，需要置SCK为高电平
  */

void MySPI_W_SCK(uint8_t BitValue)
{
	GPIO_WriteOutBits(HT_GPIOC,GPIO_PIN_2,(FlagStatus)BitValue);  //根据BitValue，设置SCK引脚的电平
}
/**
  * 函    数：SPI写MOSI引脚电平
  * 参    数：BitValue 协议层传入的当前需要写入MOSI的电平，范围0~0xFF
  * 返 回 值：无
  * 注意事项：此函数需要用户实现内容，当BitValue为0时，需要置MOSI为低电平，当BitValue非0时，需要置MOSI为高电平
  */
void MySPI_W_MOSI(uint8_t BitValue)
{
	GPIO_WriteOutBits(HT_GPIOC,GPIO_PIN_3,(FlagStatus)BitValue);  //根据BitValue，设置MOSI引脚的电平，BitValue要实现非0即1的特性
}
/**
  * 函    数：I2C读MISO引脚电平
  * 参    数：无
  * 返 回 值：协议层需要得到的当前MISO的电平，范围0~1
  * 注意事项：此函数需要用户实现内容，当前MISO为低电平时，返回0，当前MISO为高电平时，返回1
  */
uint8_t MySPI_R_MISO(void)
{
	return GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_6);  //读取MISO电平并返回
}
/**
  * 函    数：SPI初始化
  * 参    数：无
  * 返 回 值：无
  * 注意事项：此函数需要用户实现内容，实现SS、SCK、MOSI和MISO引脚的初始化
  */
void MySPI_init(void)
{
	/*开启时钟*/
		CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
    CKCUClock.Bit.PB         = 1;			//开启GPIOB的时钟
	  CKCUClock.Bit.PA         = 1;		//开启GPIOA的时钟
		CKCUClock.Bit.PC         = 1;		//开启GPIOC的时钟
		CKCUClock.Bit.AFIO         = 1;
		CKCU_PeripClockConfig(CKCUClock, ENABLE);
	 
	 AFIO_GPxConfig(GPIO_PA, GPIO_PIN_6, AFIO_MODE_DEFAULT);//config CS
	 GPIO_DirectionConfig(HT_GPIOA,GPIO_PIN_6,GPIO_DIR_OUT);
	 
	 AFIO_GPxConfig(GPIO_PC, GPIO_PIN_2, AFIO_MODE_DEFAULT);//config SCLK
	 GPIO_DirectionConfig(HT_GPIOC,GPIO_PIN_2,GPIO_DIR_OUT);
	 
	 AFIO_GPxConfig(GPIO_PC, GPIO_PIN_3, AFIO_MODE_DEFAULT);//config MOSI
	 GPIO_DirectionConfig(HT_GPIOC,GPIO_PIN_3,GPIO_DIR_OUT);
	 
	 AFIO_GPxConfig(GPIO_PB, GPIO_PIN_6, AFIO_MODE_DEFAULT);//config MISO
	 GPIO_DirectionConfig(HT_GPIOB,GPIO_PIN_6,GPIO_DIR_IN);
	 GPIO_InputConfig(HT_GPIOB,GPIO_PIN_6,ENABLE);
	 
	 /*设置默认电平*/
	 MySPI_W_SS(1);      //SS默认高电平
	 MySPI_W_SCK(0);		 //SCK默认低电平
}
/*协议层*/

/**
  * 函    数：SPI起始
  * 参    数：无
  * 返 回 值：无
  */
void MySPI_Start(void)
{
	 MySPI_W_SS(0);			//拉低SS，开始时序
}
/**
  * 函    数：SPI终止
  * 参    数：无
  * 返 回 值：无
  */
void MySPI_Stop(void)
{
	 MySPI_W_SS(1);		//拉高SS，终止时序
}
/**
  * 函    数：SPI交换传输一个字节，使用SPI模式0
  * 参    数：ByteSend 要发送的一个字节
  * 返 回 值：接收的一个字节
  */
uint8_t MySPI_SwapByte(uint8_t ByteSend)
{
	uint8_t i,ByteReceive=0x00; 		//定义接收的数据，并赋初值0x00，此处必须赋初值0x00，后面会用到
	
	for(i=0;i<8;i++)		//循环8次，依次交换每一位数据
	{
		MySPI_W_MOSI(ByteSend&(0x80>>i)); //使用掩码的方式取出ByteSend的指定一位数据并写入到MOSI线
		 MySPI_W_SCK(1);			//拉高SCK，上升沿移出数据
		if(MySPI_R_MISO()==1)
		{
			ByteReceive|=(0x80>>i);		//读取MISO数据，并存储到Byte变量
		}														//当MISO为1时，置变量指定位为1，当MISO为0时，不做处理，指定位为默认的初值0
		MySPI_W_SCK(0);								//拉低SCK，下降沿移入数据
	}
	return ByteReceive;						//返回接收到的一个字节数据
}
