#include "config.h"
#include "task.h"

/*
 *Note:In order to add new task ,we must modify 4 places :
 *1.TASK_TIMER_CNT
 *2.timeout_cb declare
 *3.task_timer_para
 *4.timeout_cb realize
*/

/*--------------------------------1.����ʱ����������----------------------------------*/
#define TASK_TIMER_CNT  4//����ʱ������,��������ʱ���޸ĸú�ֵ
struct Timer task_timer[TASK_TIMER_CNT];
/*---------------------------------------------------------------------------------------*/


/*---------------2.����ص���������(��������ʱ����Ӷ�Ӧ�Ļص���������)-----------------*/
void uart1_task_timer_callback(void);//����ص��������� ����uart1֡������ϴ���
void usart0_task_timer_callback(void);//����ص��������� ����usart0֡������ϴ���
void GPTMic_task_timer_callback(void); //GPTM_CH1���벶��
void MQTTTask_timer_callback(void);
void UploaddataTask_timer_callback(void);
void uart0_task_timer_callback(void);
void usart1_task_timer_callback(void);
void wifi_task_timer_callback(void);
void source_task_timer_callback(void);
void source2_task_timer_callback(void);
void INA226_task_timer_callback(void);
/*---------------------------------------------------------------------------------------*/

/*---------------3.����ʱ������(��������ʱ�밴���¸�ʽ��Ӷ�Ӧ������ʱ������)--------*/
task_timer_parameter task_timer_para[TASK_TIMER_CNT] =
 {
   /*{timeout_cb,				timeout(ms),		repeat(ms)}*/
	 {uart1_task_timer_callback,50, 50},
//	 {usart0_task_timer_callback,50, 50},
	 {uart0_task_timer_callback,50, 50},
//	 {wifi_task_timer_callback,200, 200},
//	 {usart1_task_timer_callback,50, 50},
//	 {MQTTTask_timer_callback,200,200},
//	 {UploaddataTask_timer_callback,10000,10000},
	 {source_task_timer_callback,1000,1000},
	 {INA226_task_timer_callback,50,50}
//	 {GPTMic_task_timer_callback,0,20}
	 
   /*{timeout_cb,				timeout(ms),		repeat(ms)}*/ 
 };
/*---------------------------------------------------------------------------------------*/       


/*--------4.����ʱ���ص�����ʵ��(��������ʱ����Ӷ�Ӧ�Ķ�ʱ���ص�����ʵ��)------------*/
void uart1_task_timer_callback(void)
{
	Uart1_RecProcess( );
		
}
void wifi_task_timer_callback(void)
{
		//WiFi_Connect_Init();
	static u8 Wifi_connect_flag1=0;
	if(Wifi_connect_flag1==0)
	{
		Wifi_connect_flag1++;
		WiFi_ResetIO_Init();            //��ʼ��WiFi�ĸ�λIO
		AliIoT_Parameter_Init();	    //��ʼ�����Ӱ�����IoTƽ̨MQTT�������Ĳ���	
		WiFi_Connect_Init();
		MQTT_Buff_Init();              //��ʼ�����ͽ���������� 
		BF_TIM_32s_int();  //����ʼ����Ϳ�ʼ����ping������䲻�ܷ�δ���ӳɹ�ǰ
	}
}
void usart0_task_timer_callback(void)
{
	Usart0_RecProcess( );

}
void uart0_task_timer_callback(void)
{
	Uart0_RecProcess( );
}
void usart1_task_timer_callback(void)
{
	Usart1_RecProcess( );
	
}
void source_task_timer_callback(void)
{
	unsigned char val=0;
	u8 flag=0;
	flag=SourceReadByte(&val);
	if(flag!=0)
	{
		printf("��ֵ��ȡʧ��");
//		ChuanKouPin_Printf("serial.slt1.txt+=\"��ֵ��ȡʧ��\r\n\"\xff\xff\xff");//�����ߵ��Խ�����ʾ
		ChuanKouPin_Printf("power.t1.txt+=\"����״̬�쳣\"\xff\xff\xff");//�����ߵ��Խ�����ʾ
	}
	else
	{
		printf("��ֵ��ȡ�ɹ�");
		printf("val=%d\r\n",val);
//		ChuanKouPin_Printf("serial.slt1.txt+=\"��ֵ��ȡ�ɹ�\r\n\"\xff\xff\xff");//�����ߵ��Խ�����ʾ
		ChuanKouPin_Printf("power.t1.txt+=\"����״̬����\"\xff\xff\xff");//�����ߵ��Խ�����ʾ
	}
	val=127;
	flag=SourceWriteByte(val);
	if(flag!=0)
	{
		printf("д����ʧ��\r\n");
		ChuanKouPin_Printf("power.t1.txt+=\"��ѹ�޸��쳣\"\xff\xff\xff");//�����ߵ��Խ�����ʾ
	}
	else
	{
		printf("д����ɹ�\r\n");
		printf("val=%d\r\n",val);	
		ChuanKouPin_Printf("power.t1.txt+=\"��ѹ�޸�����\"\xff\xff\xff");//�����ߵ��Խ�����ʾ
	}
}
void INA226_task_timer_callback(void)
{
	float V_temp=0;
	float V_RealValue=0;
	V_temp = INA226_ReadData(BV_REG);
	V_RealValue=V_temp;
	//printf("V:%.2f\r\n",V_RealValue*0.00125f); //����ʵ�����ߵ�ѹ);
	UART0_printf("add checkV.s0.id,0,%d\xff\xff\xff",(int)((V_RealValue*0.00125f)*10));//��������ӡ
	int voltage_integer = (int)V_temp*0.00125f;
  int voltage_decimal = (int)((V_RealValue*0.00125f - voltage_integer) * 1000);
  //printf("V: ��������: %d, С�������λ: %d\r\n", voltage_integer, voltage_decimal);
	UART0_printf("checkV.t0.txt=\"%d\"\xff\xff\xff",voltage_integer);
	UART0_printf("checkV.t2.txt=\"%d\"\xff\xff\xff",voltage_decimal);
	
	float temp=0;
	temp = INA226_ReadData(SV_REG);
	int current_integer = (int)temp;
  int current_decimal = (int)((temp - current_integer) * 100);
  //printf("A: ��������: %d, С�������λ: %02d\r\n", current_integer, current_decimal);
	//printf("A:%.6f\r\n",temp*0.0000592857f); //����ʵ�����ߵ�ѹ);
	//UART0_printf("add power.s1.id,0,%f\xff\xff\xff",V_RealValue*0.00125f);//��������ӡ
//	UART0_printf("add power.s0.id,1,%d\xff\xff\xff",(int)((V_RealValue*0.00125f)+1));//��������ӡ
//	UART0_printf("power.t13.txt=\"%d\xff\xff\xff",current_integer);
//	UART0_printf("power.t22.txt=\"%d\xff\xff\xff",current_decimal);
	
}

void GPTMic_task_timer_callback(void)
{
	 if (CaptureCHx.DataValid)
    {
      printf("Capture : %u uS\r\n", (int)(CaptureCHx.CapturePulse / (SystemCoreClock/1000000ul)));
      CaptureCHx.DataValid = FALSE;
    }
}

void MQTTTask_timer_callback(void)
{
			if(Connect_flag==1){     
			/*-------------------------------------------------------------*/
			/*                     �����ͻ���������                      */
			/*-------------------------------------------------------------*/
			if(MQTT_TxDataOutPtr != MQTT_TxDataInPtr){                    //if�����Ļ���˵�����ͻ�������������
				//3������ɽ���if
				//��1�֣�0x10 ���ӱ���
				//��2�֣�0x82 ���ı��ģ���ConnectPack_flag��λ����ʾ���ӱ��ĳɹ�
				//��3�֣�SubcribePack_flag��λ��˵�����ӺͶ��ľ��ɹ����������Ŀɷ�
				if((MQTT_TxDataOutPtr[2]==0x10)||((MQTT_TxDataOutPtr[2]==0x82)&&(ConnectPack_flag==1))||(SubcribePack_flag==1)){    
					printf("��������:0x%x\r\n",MQTT_TxDataOutPtr[2]);  //������ʾ��Ϣ
					MQTT_TxData(MQTT_TxDataOutPtr);                       //��������
					MQTT_TxDataOutPtr += TBUFF_UNIT;                      //ָ������
					if(MQTT_TxDataOutPtr==MQTT_TxDataEndPtr)              //���ָ�뵽������β����
						MQTT_TxDataOutPtr = MQTT_TxDataBuf[0];            //ָ���λ����������ͷ
				} 				
			}//�����ͻ��������ݵ�else if��֧��β
			/*-------------------------------------------------------------*/
			/*                     ������ջ���������                      */
			/*-------------------------------------------------------------*/
			if(MQTT_RxDataOutPtr != MQTT_RxDataInPtr){                //if�����Ļ���˵�����ջ�������������														
				printf("���յ�����:");                             //������ʾ��Ϣ
				/*-----------------------------------------------------*/
				/*                    ����CONNACK����                  */
				/*-----------------------------------------------------*/				
				//if�жϣ������һ���ֽ���0x20����ʾ�յ�����CONNACK����
				//��������Ҫ�жϵ�4���ֽڣ�����CONNECT�����Ƿ�ɹ�
				if(MQTT_RxDataOutPtr[2]==0x20){             			
				    switch(MQTT_RxDataOutPtr[5]){					
						case 0x00 : printf("CONNECT���ĳɹ�\r\n");                            //���������Ϣ	
								    ConnectPack_flag = 1;                                        //CONNECT���ĳɹ�
									break;                                                       //������֧case 0x00                                              
						case 0x01 : printf("�����Ѿܾ�����֧�ֵ�Э��汾��׼������\r\n");     //���������Ϣ
									Connect_flag = 0;                                            //Connect_flag���㣬��������
									break;                                                       //������֧case 0x01   
						case 0x02 : printf("�����Ѿܾ������ϸ�Ŀͻ��˱�ʶ����׼������\r\n"); //���������Ϣ
									Connect_flag = 0;                                            //Connect_flag���㣬��������
									break;                                                       //������֧case 0x02 
						case 0x03 : printf("�����Ѿܾ�������˲����ã�׼������\r\n");         //���������Ϣ
									Connect_flag = 0;                                            //Connect_flag���㣬��������
									break;                                                       //������֧case 0x03
						case 0x04 : printf("�����Ѿܾ�����Ч���û��������룬׼������\r\n");   //���������Ϣ
									Connect_flag = 0;                                            //Connect_flag���㣬��������						
									break;                                                       //������֧case 0x04
						case 0x05 : printf("�����Ѿܾ���δ��Ȩ��׼������\r\n");               //���������Ϣ
									Connect_flag = 0;                                            //Connect_flag���㣬��������						
									break;                                                       //������֧case 0x05 		
						default   : printf("�����Ѿܾ���δ֪״̬��׼������\r\n");             //���������Ϣ 
									Connect_flag = 0;                                            //Connect_flag���㣬��������					
									break;                                                       //������֧case default 								
					}				
				}
				/*-----------------------------------------------------*/
				/*                    ����SUBACK����                   */
				/*-----------------------------------------------------*/				
				//if�жϣ���һ���ֽ���0x90����ʾ�յ�����SUBACK����
				//��������Ҫ�ж϶��Ļظ��������ǲ��ǳɹ�
				else if(MQTT_RxDataOutPtr[2]==0x90){ 
						switch(MQTT_RxDataOutPtr[6]){					
						case 0x00 :
						case 0x01 : printf("���ĳɹ�\r\n");            //���������Ϣ
							        SubcribePack_flag = 1;                //SubcribePack_flag��1����ʾ���ı��ĳɹ���
									break;                                //������֧                                             
						default   : printf("����ʧ�ܣ�׼������\r\n");  //���������Ϣ 
									Connect_flag = 0;                     //Connect_flag���㣬��������
									break;                                //������֧ 								
					}					
				}
				/*-----------------------------------------------------*/
				/*                  ����PINGRESP����                   */
				/*-----------------------------------------------------*/
				//if�жϣ���һ���ֽ���0xD0����ʾ�յ�����PINGRESP����
				else if(MQTT_RxDataOutPtr[2]==0xD0){ 
					printf("PING���Ļظ�\r\n"); 		  //���������Ϣ 
					UART0_printf("wifi.slt1.txt+=\"PING���Ļظ�\r\n\"\xff\xff\xff");
					if(Ping_flag==1){                     //���Ping_flag=1����ʾ��һ�η���
						 Ping_flag = 0;    				  //Ҫ���Ping_flag��־
					}else if(Ping_flag>1){ 				  //���Ping_flag>1����ʾ�Ƕ�η����ˣ�������2s����Ŀ��ٷ���
						Ping_flag = 0;     				  //Ҫ���Ping_flag��־
						BF_TIM_32s_int(); 				  //PING��ʱ���ػ�32s��ʱ��
					}				
				}
				/*-----------------------------------------------------*/
				/*                  �����������ͱ���                   */
				/*-----------------------------------------------------*/				
				//if�жϣ������һ���ֽ���0x30����ʾ�յ����Ƿ�������������������
				//����Ҫ��ȡ��������
				else if(MQTT_RxDataOutPtr[2]==0x30){ 
					printf("�������ȼ�0����\r\n"); 		   //���������Ϣ 
					MQTT_DealPushdata_Qs0(MQTT_RxDataOutPtr);  //����ȼ�0��������
				}				
				
				MQTT_RxDataOutPtr +=RBUFF_UNIT;                     //����ָ������
				if(MQTT_RxDataOutPtr==MQTT_RxDataEndPtr)            //�������ָ�뵽���ջ�����β����
					MQTT_RxDataOutPtr = MQTT_RxDataBuf[0];          //����ָ���λ�����ջ�������ͷ                        
			}
			
			/*-------------------------------------------------------------*/
			/*                     ���������������                      */
			/*-------------------------------------------------------------*/
			if(MQTT_CMDOutPtr != MQTT_CMDInPtr){                             //if�����Ļ���˵�����������������			       
				printf("����:%s\r\n",&MQTT_CMDOutPtr[2]);                 //���������Ϣ
				if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"Switch1\":1}")){	           //���������"params":{"PowerSwitch":1}˵���������·��򿪿���1	
//					LED1_ON;                                                                   //��LED1
//					AT24C02_SaveLEDSta();                                                      //����LED��״̬��24C02
//					LED1_State();                                                              //�жϿ���״̬����������������  
				}else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"Switch1\":0}")){       //���������"params":{"PowerSwitch":0}˵���������·��رտ���1
//					LED1_OFF;                                                                  //�ر�LED1
//					AT24C02_SaveLEDSta();                                                      //����LED��״̬��24C02
//					LED1_State();                                                              //�жϿ���״̬����������������  
				}else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"Switch2\":1}")){ 	   //���������"params":{"PowerSwitch_2":1}˵���������·��򿪿���2	
//					LED2_ON;                                                                   //��LED2
//					AT24C02_SaveLEDSta();                                                      //����LED��״̬��24C02
//					LED2_State();                                                              //�жϿ���״̬����������������  
				}else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"Switch2\":0}")){       //���������"params":{"PowerSwitch_2":0}˵���������·��رտ���2
//					LED2_OFF;                                                                  //�ر�LED2
//					AT24C02_SaveLEDSta();                                                      //����LED��״̬��24C02
//					LED2_State();                                                              //�жϿ���״̬����������������  
				}else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"Switch3\":1}")){       //���������"params":{"PowerSwitch_3":1}˵���������·��򿪿���3	
//					LED3_ON;                                                                   //��LED3
//					AT24C02_SaveLEDSta();                                                      //����LED��״̬��24C02
//					LED3_State();                                                              //�жϿ���״̬����������������  
				}else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"Switch3\":0}")){       //���������"params":{"PowerSwitch_3":0}˵���������·��رտ���3
//					LED3_OFF;                                                                  //�ر�LED3
//					AT24C02_SaveLEDSta();                                                      //����LED��״̬��24C02
//					LED3_State();                                                              //�жϿ���״̬����������������  
				}else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"Switch4\":1}")){       //���������"params":{"PowerSwitch_4":1}˵���������·��򿪿���4	
//					LED4_ON;                                                                   //��LED4
//					AT24C02_SaveLEDSta();                                                      //����LED��״̬��24C02
//					LED4_State();                                                              //�жϿ���״̬����������������  
				}else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"Switch4\":0}")){       //���������"params":{"PowerSwitch_4":0}˵���������·��رտ���4
//					LED4_OFF;                                                                  //�ر�LED4
//					AT24C02_SaveLEDSta();                                                      //����LED��״̬��24C02
//					LED4_State();                                                              //�жϿ���״̬����������������  
				}				
				MQTT_CMDOutPtr += CBUFF_UNIT;                             	 //ָ������
				if(MQTT_CMDOutPtr==MQTT_CMDEndPtr)           	             //���ָ�뵽������β����
					MQTT_CMDOutPtr = MQTT_CMDBuf[0];          	             //ָ���λ����������ͷ				
			}//��������������ݵ�else if��֧��β	
		}

}

void UploaddataTask_timer_callback(void)
{
	if(Connect_flag==1)
	{ 
		double tempdata=20.5,humidata=55.3;	//���ڱ�����ʪ�ȵı���
		float  adcdata[3]={1.2,2.3,2.1};          //���ڱ���3��ADCͨ��������	
		char temp[256];             //������
		//char test[256];
		u8  i=0;
		static u8 j=0;                    
		if(j%3==0)
		{
	//		AHT10_Data(&tempdata,&humidata);                                   //��ȡ��ʪ��
			//printf("�¶ȣ�%.2f  ʪ�ȣ�%.2f\r\n",tempdata,humidata);         //�����������
			//UART0_printf("wifi.slt1.txt+=\"�¶ȣ�%.2f  ʪ�ȣ�%.2f\r\n\r\n\"\xff\xff\xff",tempdata,humidata );
			sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"humi\":%d,\"temp\":%d},\"version\":\"1.0.0\"}",(int)humidata,(int)tempdata);  //��������
			MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));                  //������ݵ����ͻ�����
			j=0;
		}
		
		for(i=0;i<3;i++){
//			adcdata[i] = (float)(Get_Adc_Average(i+5,20))*(3.3/4096);     //��ȡ�����Ĵ�С����ʵ�ʵ�ѹֵ
			printf("ADC%d��%.2f\r\n",i,adcdata[i]);                    //���������Ϣ
		}
		//sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"led\":%d},\"version\":\"1.0.0\"}",(int)adcdata[0]);  //��������
		UART0_printf("wifi.slt1.txt+=\"This message from esp01s\r\n\r\n\"\xff\xff\xff");
		sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"Text\":\"This masseage from esp01s\"},\"version\":\"1.0.0\"}");
		MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));                  //������ݣ�������������	
		//MQTT_PublishQs0(P_TOPIC_NAME,test,strlen(test)); 
    j++;
	}
	
}







/*---------------------------------------------------------------------------------------*/


/*--�����ʼ��task_init(sys_init()�е���)�����к���task_run(main.c�е���)�������������޸�--*/

int task_init(void)
{   
	uint8_t task_index;
	for(task_index = 0;task_index < TASK_TIMER_CNT;task_index++)
	{
	    timer_init(&task_timer[task_index],
		           task_timer_para[task_index].timeout_cb, 
		           task_timer_para[task_index].timeout,
		           task_timer_para[task_index].repeat); 
		
	    timer_start(&task_timer[task_index]);
	}
	return 0;
}


/*---------------------------------------------------------------------------------------*/
//ϵͳ��Ϣ����
static uint8_t message_buff[256];
//uint8_t wifi_temp_buff[256];
char blut_Buf[512];
char Serial_Buf[512];
char Res_ValueBuf[8];
u32 Serial_BTL=0;
u8 Res_Value=0;
void msg_process(void)  //ϵͳ��Ϣ����
{
	system_message sys_msg;	
	if(rt_ringbuffer_data_len(&msg_ring_buf)!= 0) //��Ϣ���в�Ϊ�գ�������Ϣ��Ҫ����
	{
		rt_ringbuffer_get(&msg_ring_buf,(uint8_t *) &sys_msg.msg_type, 2);//��ȡ��Ϣ����(1�ֽ�) + ��Ϣ����(1�ֽ�)�����У���ͬ��Ϣ���Ͷ�Ӧ����Ϣ��������Ҳ��ͬ�����û����ж���
		
		switch(sys_msg.msg_type)
		{
			case MSG_UART1_REC_FINISH://����
				
				  rt_ringbuffer_get(&uart1_recv_ring_buf, message_buff, sys_msg.msg_parameter);
			    message_buff[sys_msg.msg_parameter] = '\0';//sys_msg.msg_parameter�д�ŵ��Ǵ���1����֡�ĳ�����Ϣ
			
			    //YouXian_Printf("uart1 recv:%s\n",message_buff);
					ChuanKouPin_Printf("serial.slt1.txt+=\"%s\r\n\"\xff\xff\xff",message_buff);
				break;
      case MSG_USART0_REC_FINISH://wifi
//				  rt_ringbuffer_get(&usart0_recv_ring_buf, message_buff, sys_msg.msg_parameter);
//			    message_buff[sys_msg.msg_parameter] = '\0';
//			    printf("usart0 recv:%s\n",message_buff);
				break;
			case MSG_UART0_REC_FINISH://������
				  rt_ringbuffer_get(&uart0_recv_ring_buf, message_buff, sys_msg.msg_parameter);
			    message_buff[sys_msg.msg_parameter] = '\0';//sys_msg.msg_parameter�д�ŵ��Ǵ���0����֡�ĳ�����Ϣ
					//ͨ�����������������ߵ��ԵĲ�����
					if(strstr((char*)message_buff,"BTLSET")!=NULL)
						{
							char *ptr = strstr((char*)message_buff, "BTLSET");
							ptr += strlen("BTLSET");
							sscanf(ptr, "%[^\n]", Serial_Buf);//��ֵ
							Serial_BTL=atoi(Serial_Buf);
							ChuanKouPin_Printf("serial.slt1.txt+=\"%s\r\n\"\xff\xff\xff",Serial_Buf);//�����ߵ��Խ�����ʾ
							YouXian_Printf("�����ʣ�%d",Serial_BTL);
							s_UART1_RecFrameLen=0;
							USART_TxCmd(HT_UART1, DISABLE);
							USART_RxCmd(HT_UART1, DISABLE);
							Uart1_Init(Serial_BTL);//���ߵ��ԵĲ�����
							USART_TxCmd(HT_UART1, ENABLE);
							USART_RxCmd(HT_UART1, ENABLE);
						}
						else if(strstr((char*)message_buff,"ResSet")!=NULL)
						{
							int Temp_Flag=0;
							char *ptr = strstr((char*)message_buff, "ResSet");
							ptr += strlen("ResSet");
							sscanf(ptr, "%[^\n]", Res_ValueBuf);//��ֵ
							Res_Value=atoi(Res_ValueBuf);
							SourceWriteByte(Res_Value);
							Temp_Flag=SourceWriteByte(Res_Value);
							if(Temp_Flag!=0)
							{
								//��ֵ����ʧ�ܣ��������Ͻ��ı����ֵ��ʾ�ڶ�Ӧ��λ��
								ChuanKouPin_Printf("power.t1.txt+=\"����״̬�쳣\"\xff\xff\xff");//�����ߵ��Խ�����ʾ
							}
							else 
							{
								//��ֵ����ʧ�ܣ��������ϵ�Դ������״̬Ϊ�쳣
								ChuanKouPin_Printf("power.t1.txt+=\"����״̬����\"\xff\xff\xff");//�����ߵ��Խ�����ʾ
							}
						}
				break;
					case MSG_USART1_REC_FINISH://usart1����
					rt_ringbuffer_get(&usart1_recv_ring_buf, message_buff, sys_msg.msg_parameter);
			    message_buff[sys_msg.msg_parameter] = '\0';//sys_msg.msg_parameter�д�ŵ��Ǵ���0����֡�ĳ�����Ϣ
			   //YouXian_Printf("usart1 recv:%s\n",message_buff);
				break;
			default:break;
		}	
		
	}
	
}
/*---------------------------------------------------------------------------------------*/

int task_run(void)
{
	timer_loop();  //��ʱ�����ѯ��ִ��
	msg_process(); //ϵͳ��Ϣ����
	return 0;
}
/*---------------------------------------------------------------------------------------*/
