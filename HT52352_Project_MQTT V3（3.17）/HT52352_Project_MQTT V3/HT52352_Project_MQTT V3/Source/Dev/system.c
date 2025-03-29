#include "config.h"
#include "system.h"

#define MSG_POOL_CAPACITY 1024
static uint8_t msg_pool[MSG_POOL_CAPACITY];//���ζ�����Ϣ��
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
//	WiFi_ResetIO_Init();            //��ʼ��WiFi�ĸ�λIO
//	
//	AliIoT_Parameter_Init();	    //��ʼ�����Ӱ�����IoTƽ̨MQTT�������Ĳ���	
//	WiFi_Connect_Init();
//	MQTT_Buff_Init();              //��ʼ�����ͽ���������� 
//	BF_TIM_32s_int();  //����ʼ����Ϳ�ʼ����ping������䲻�ܷ�δ���ӳɹ�ǰ
	rt_ringbuffer_init(&msg_ring_buf, msg_pool, MSG_POOL_CAPACITY); //��ʼ����Ϣ���ζ���
	task_init();//�����ʼ��
}


