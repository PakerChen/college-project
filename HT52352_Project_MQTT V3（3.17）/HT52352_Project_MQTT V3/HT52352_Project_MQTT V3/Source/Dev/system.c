#include "config.h"
#include "system.h"

#define MSG_POOL_CAPACITY 1024
static uint8_t msg_pool[MSG_POOL_CAPACITY];//环形队列消息池
void SysInit (void)
{
	//Led_Init();
	SysTickInit();
	
	Uart1_Init(115200);//(PB4:TX,PB5:RX)
//  Usart0_Init(115200);//(PA2:TX,PA3:RX)
	Uart0_Init(115200);//(TX:PC4,RX:PC5)
//	Usart1_Init(115200);//(TX:PA4,RX:PA5)
  //SCTM_Configuration();
		I2C_Initia();//SCL B0 SDA B1
		INA226_Init();//SCL B7 SDA B8
		W25Q64_Init();
//	WiFi_ResetIO_Init();            //初始化WiFi的复位IO
//	
//	AliIoT_Parameter_Init();	    //初始化连接阿里云IoT平台MQTT服务器的参数	
//	WiFi_Connect_Init();
//	MQTT_Buff_Init();              //初始化发送接收命令缓冲区 
//	BF_TIM_32s_int();  //这句初始化后就开始发送ping包，这句不能放未连接成功前
	rt_ringbuffer_init(&msg_ring_buf, msg_pool, MSG_POOL_CAPACITY); //初始化消息环形队列
	task_init();//任务初始化
}


