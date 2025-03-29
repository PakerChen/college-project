#include "config.h"
#include "task.h"

/*
 *Note:In order to add new task ,we must modify 4 places :
 *1.TASK_TIMER_CNT
 *2.timeout_cb declare
 *3.task_timer_para
 *4.timeout_cb realize
*/

/*--------------------------------1.任务定时器变量定义----------------------------------*/
#define TASK_TIMER_CNT  4//任务定时器个数,增加任务时须修改该宏值
struct Timer task_timer[TASK_TIMER_CNT];
/*---------------------------------------------------------------------------------------*/


/*---------------2.任务回调函数声明(增加任务时须添加对应的回调函数声明)-----------------*/
void uart1_task_timer_callback(void);//任务回调函数声明 串口uart1帧接收完毕处理
void usart0_task_timer_callback(void);//任务回调函数声明 串口usart0帧接收完毕处理
void GPTMic_task_timer_callback(void); //GPTM_CH1输入捕获
void MQTTTask_timer_callback(void);
void UploaddataTask_timer_callback(void);
void uart0_task_timer_callback(void);
void usart1_task_timer_callback(void);
void wifi_task_timer_callback(void);
void source_task_timer_callback(void);
void source2_task_timer_callback(void);
void INA226_task_timer_callback(void);
/*---------------------------------------------------------------------------------------*/

/*---------------3.任务定时器参数(增加任务时须按如下格式添加对应的任务定时器参数)--------*/
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


/*--------4.任务定时器回调函数实现(增加任务时须添加对应的定时器回调函数实现)------------*/
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
		WiFi_ResetIO_Init();            //初始化WiFi的复位IO
		AliIoT_Parameter_Init();	    //初始化连接阿里云IoT平台MQTT服务器的参数	
		WiFi_Connect_Init();
		MQTT_Buff_Init();              //初始化发送接收命令缓冲区 
		BF_TIM_32s_int();  //这句初始化后就开始发送ping包，这句不能放未连接成功前
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
		printf("阻值读取失败");
//		ChuanKouPin_Printf("serial.slt1.txt+=\"阻值读取失败\r\n\"\xff\xff\xff");//在有线调试界面显示
		ChuanKouPin_Printf("power.t1.txt+=\"运行状态异常\"\xff\xff\xff");//在有线调试界面显示
	}
	else
	{
		printf("阻值读取成功");
		printf("val=%d\r\n",val);
//		ChuanKouPin_Printf("serial.slt1.txt+=\"阻值读取成功\r\n\"\xff\xff\xff");//在有线调试界面显示
		ChuanKouPin_Printf("power.t1.txt+=\"运行状态正常\"\xff\xff\xff");//在有线调试界面显示
	}
	val=127;
	flag=SourceWriteByte(val);
	if(flag!=0)
	{
		printf("写电阻失败\r\n");
		ChuanKouPin_Printf("power.t1.txt+=\"电压修改异常\"\xff\xff\xff");//在有线调试界面显示
	}
	else
	{
		printf("写电阻成功\r\n");
		printf("val=%d\r\n",val);	
		ChuanKouPin_Printf("power.t1.txt+=\"电压修改正常\"\xff\xff\xff");//在有线调试界面显示
	}
}
void INA226_task_timer_callback(void)
{
	float V_temp=0;
	float V_RealValue=0;
	V_temp = INA226_ReadData(BV_REG);
	V_RealValue=V_temp;
	//printf("V:%.2f\r\n",V_RealValue*0.00125f); //计算实际总线电压);
	UART0_printf("add checkV.s0.id,0,%d\xff\xff\xff",(int)((V_RealValue*0.00125f)*10));//串口屏打印
	int voltage_integer = (int)V_temp*0.00125f;
  int voltage_decimal = (int)((V_RealValue*0.00125f - voltage_integer) * 1000);
  //printf("V: 整数部分: %d, 小数点后两位: %d\r\n", voltage_integer, voltage_decimal);
	UART0_printf("checkV.t0.txt=\"%d\"\xff\xff\xff",voltage_integer);
	UART0_printf("checkV.t2.txt=\"%d\"\xff\xff\xff",voltage_decimal);
	
	float temp=0;
	temp = INA226_ReadData(SV_REG);
	int current_integer = (int)temp;
  int current_decimal = (int)((temp - current_integer) * 100);
  //printf("A: 整数部分: %d, 小数点后两位: %02d\r\n", current_integer, current_decimal);
	//printf("A:%.6f\r\n",temp*0.0000592857f); //计算实际总线电压);
	//UART0_printf("add power.s1.id,0,%f\xff\xff\xff",V_RealValue*0.00125f);//串口屏打印
//	UART0_printf("add power.s0.id,1,%d\xff\xff\xff",(int)((V_RealValue*0.00125f)+1));//串口屏打印
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
			/*                     处理发送缓冲区数据                      */
			/*-------------------------------------------------------------*/
			if(MQTT_TxDataOutPtr != MQTT_TxDataInPtr){                    //if成立的话，说明发送缓冲区有数据了
				//3种情况可进入if
				//第1种：0x10 连接报文
				//第2种：0x82 订阅报文，且ConnectPack_flag置位，表示连接报文成功
				//第3种：SubcribePack_flag置位，说明连接和订阅均成功，其他报文可发
				if((MQTT_TxDataOutPtr[2]==0x10)||((MQTT_TxDataOutPtr[2]==0x82)&&(ConnectPack_flag==1))||(SubcribePack_flag==1)){    
					printf("发送数据:0x%x\r\n",MQTT_TxDataOutPtr[2]);  //串口提示信息
					MQTT_TxData(MQTT_TxDataOutPtr);                       //发送数据
					MQTT_TxDataOutPtr += TBUFF_UNIT;                      //指针下移
					if(MQTT_TxDataOutPtr==MQTT_TxDataEndPtr)              //如果指针到缓冲区尾部了
						MQTT_TxDataOutPtr = MQTT_TxDataBuf[0];            //指针归位到缓冲区开头
				} 				
			}//处理发送缓冲区数据的else if分支结尾
			/*-------------------------------------------------------------*/
			/*                     处理接收缓冲区数据                      */
			/*-------------------------------------------------------------*/
			if(MQTT_RxDataOutPtr != MQTT_RxDataInPtr){                //if成立的话，说明接收缓冲区有数据了														
				printf("接收到数据:");                             //串口提示信息
				/*-----------------------------------------------------*/
				/*                    处理CONNACK报文                  */
				/*-----------------------------------------------------*/				
				//if判断，如果第一个字节是0x20，表示收到的是CONNACK报文
				//接着我们要判断第4个字节，看看CONNECT报文是否成功
				if(MQTT_RxDataOutPtr[2]==0x20){             			
				    switch(MQTT_RxDataOutPtr[5]){					
						case 0x00 : printf("CONNECT报文成功\r\n");                            //串口输出信息	
								    ConnectPack_flag = 1;                                        //CONNECT报文成功
									break;                                                       //跳出分支case 0x00                                              
						case 0x01 : printf("连接已拒绝，不支持的协议版本，准备重启\r\n");     //串口输出信息
									Connect_flag = 0;                                            //Connect_flag置零，重启连接
									break;                                                       //跳出分支case 0x01   
						case 0x02 : printf("连接已拒绝，不合格的客户端标识符，准备重启\r\n"); //串口输出信息
									Connect_flag = 0;                                            //Connect_flag置零，重启连接
									break;                                                       //跳出分支case 0x02 
						case 0x03 : printf("连接已拒绝，服务端不可用，准备重启\r\n");         //串口输出信息
									Connect_flag = 0;                                            //Connect_flag置零，重启连接
									break;                                                       //跳出分支case 0x03
						case 0x04 : printf("连接已拒绝，无效的用户名或密码，准备重启\r\n");   //串口输出信息
									Connect_flag = 0;                                            //Connect_flag置零，重启连接						
									break;                                                       //跳出分支case 0x04
						case 0x05 : printf("连接已拒绝，未授权，准备重启\r\n");               //串口输出信息
									Connect_flag = 0;                                            //Connect_flag置零，重启连接						
									break;                                                       //跳出分支case 0x05 		
						default   : printf("连接已拒绝，未知状态，准备重启\r\n");             //串口输出信息 
									Connect_flag = 0;                                            //Connect_flag置零，重启连接					
									break;                                                       //跳出分支case default 								
					}				
				}
				/*-----------------------------------------------------*/
				/*                    处理SUBACK报文                   */
				/*-----------------------------------------------------*/				
				//if判断，第一个字节是0x90，表示收到的是SUBACK报文
				//接着我们要判断订阅回复，看看是不是成功
				else if(MQTT_RxDataOutPtr[2]==0x90){ 
						switch(MQTT_RxDataOutPtr[6]){					
						case 0x00 :
						case 0x01 : printf("订阅成功\r\n");            //串口输出信息
							        SubcribePack_flag = 1;                //SubcribePack_flag置1，表示订阅报文成功送
									break;                                //跳出分支                                             
						default   : printf("订阅失败，准备重启\r\n");  //串口输出信息 
									Connect_flag = 0;                     //Connect_flag置零，重启连接
									break;                                //跳出分支 								
					}					
				}
				/*-----------------------------------------------------*/
				/*                  处理PINGRESP报文                   */
				/*-----------------------------------------------------*/
				//if判断，第一个字节是0xD0，表示收到的是PINGRESP报文
				else if(MQTT_RxDataOutPtr[2]==0xD0){ 
					printf("PING报文回复\r\n"); 		  //串口输出信息 
					UART0_printf("wifi.slt1.txt+=\"PING报文回复\r\n\"\xff\xff\xff");
					if(Ping_flag==1){                     //如果Ping_flag=1，表示第一次发送
						 Ping_flag = 0;    				  //要清除Ping_flag标志
					}else if(Ping_flag>1){ 				  //如果Ping_flag>1，表示是多次发送了，而且是2s间隔的快速发送
						Ping_flag = 0;     				  //要清除Ping_flag标志
						BF_TIM_32s_int(); 				  //PING定时器重回32s的时间
					}				
				}
				/*-----------------------------------------------------*/
				/*                  处理数据推送报文                   */
				/*-----------------------------------------------------*/				
				//if判断，如果第一个字节是0x30，表示收到的是服务器发来的推送数据
				//我们要提取控制命令
				else if(MQTT_RxDataOutPtr[2]==0x30){ 
					printf("服务器等级0推送\r\n"); 		   //串口输出信息 
					MQTT_DealPushdata_Qs0(MQTT_RxDataOutPtr);  //处理等级0推送数据
				}				
				
				MQTT_RxDataOutPtr +=RBUFF_UNIT;                     //接收指针下移
				if(MQTT_RxDataOutPtr==MQTT_RxDataEndPtr)            //如果接收指针到接收缓冲区尾部了
					MQTT_RxDataOutPtr = MQTT_RxDataBuf[0];          //接收指针归位到接收缓冲区开头                        
			}
			
			/*-------------------------------------------------------------*/
			/*                     处理命令缓冲区数据                      */
			/*-------------------------------------------------------------*/
			if(MQTT_CMDOutPtr != MQTT_CMDInPtr){                             //if成立的话，说明命令缓冲区有数据了			       
				printf("命令:%s\r\n",&MQTT_CMDOutPtr[2]);                 //串口输出信息
				if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"Switch1\":1}")){	           //如果搜索到"params":{"PowerSwitch":1}说明服务器下发打开开关1	
//					LED1_ON;                                                                   //打开LED1
//					AT24C02_SaveLEDSta();                                                      //保存LED的状态到24C02
//					LED1_State();                                                              //判断开关状态，并发布给服务器  
				}else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"Switch1\":0}")){       //如果搜索到"params":{"PowerSwitch":0}说明服务器下发关闭开关1
//					LED1_OFF;                                                                  //关闭LED1
//					AT24C02_SaveLEDSta();                                                      //保存LED的状态到24C02
//					LED1_State();                                                              //判断开关状态，并发布给服务器  
				}else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"Switch2\":1}")){ 	   //如果搜索到"params":{"PowerSwitch_2":1}说明服务器下发打开开关2	
//					LED2_ON;                                                                   //打开LED2
//					AT24C02_SaveLEDSta();                                                      //保存LED的状态到24C02
//					LED2_State();                                                              //判断开关状态，并发布给服务器  
				}else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"Switch2\":0}")){       //如果搜索到"params":{"PowerSwitch_2":0}说明服务器下发关闭开关2
//					LED2_OFF;                                                                  //关闭LED2
//					AT24C02_SaveLEDSta();                                                      //保存LED的状态到24C02
//					LED2_State();                                                              //判断开关状态，并发布给服务器  
				}else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"Switch3\":1}")){       //如果搜索到"params":{"PowerSwitch_3":1}说明服务器下发打开开关3	
//					LED3_ON;                                                                   //打开LED3
//					AT24C02_SaveLEDSta();                                                      //保存LED的状态到24C02
//					LED3_State();                                                              //判断开关状态，并发布给服务器  
				}else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"Switch3\":0}")){       //如果搜索到"params":{"PowerSwitch_3":0}说明服务器下发关闭开关3
//					LED3_OFF;                                                                  //关闭LED3
//					AT24C02_SaveLEDSta();                                                      //保存LED的状态到24C02
//					LED3_State();                                                              //判断开关状态，并发布给服务器  
				}else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"Switch4\":1}")){       //如果搜索到"params":{"PowerSwitch_4":1}说明服务器下发打开开关4	
//					LED4_ON;                                                                   //打开LED4
//					AT24C02_SaveLEDSta();                                                      //保存LED的状态到24C02
//					LED4_State();                                                              //判断开关状态，并发布给服务器  
				}else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"Switch4\":0}")){       //如果搜索到"params":{"PowerSwitch_4":0}说明服务器下发关闭开关4
//					LED4_OFF;                                                                  //关闭LED4
//					AT24C02_SaveLEDSta();                                                      //保存LED的状态到24C02
//					LED4_State();                                                              //判断开关状态，并发布给服务器  
				}				
				MQTT_CMDOutPtr += CBUFF_UNIT;                             	 //指针下移
				if(MQTT_CMDOutPtr==MQTT_CMDEndPtr)           	             //如果指针到缓冲区尾部了
					MQTT_CMDOutPtr = MQTT_CMDBuf[0];          	             //指针归位到缓冲区开头				
			}//处理命令缓冲区数据的else if分支结尾	
		}

}

void UploaddataTask_timer_callback(void)
{
	if(Connect_flag==1)
	{ 
		double tempdata=20.5,humidata=55.3;	//用于保存温湿度的变量
		float  adcdata[3]={1.2,2.3,2.1};          //用于保存3个ADC通道的数据	
		char temp[256];             //缓冲区
		//char test[256];
		u8  i=0;
		static u8 j=0;                    
		if(j%3==0)
		{
	//		AHT10_Data(&tempdata,&humidata);                                   //读取温湿度
			//printf("温度：%.2f  湿度：%.2f\r\n",tempdata,humidata);         //串口输出数据
			//UART0_printf("wifi.slt1.txt+=\"温度：%.2f  湿度：%.2f\r\n\r\n\"\xff\xff\xff",tempdata,humidata );
			sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"humi\":%d,\"temp\":%d},\"version\":\"1.0.0\"}",(int)humidata,(int)tempdata);  //构建数据
			MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));                  //添加数据到发送缓冲区
			j=0;
		}
		
		for(i=0;i<3;i++){
//			adcdata[i] = (float)(Get_Adc_Average(i+5,20))*(3.3/4096);     //获取计算后的带小数的实际电压值
			printf("ADC%d：%.2f\r\n",i,adcdata[i]);                    //串口输出信息
		}
		//sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"led\":%d},\"version\":\"1.0.0\"}",(int)adcdata[0]);  //构建数据
		UART0_printf("wifi.slt1.txt+=\"This message from esp01s\r\n\r\n\"\xff\xff\xff");
		sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"Text\":\"This masseage from esp01s\"},\"version\":\"1.0.0\"}");
		MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));                  //添加数据，发布给服务器	
		//MQTT_PublishQs0(P_TOPIC_NAME,test,strlen(test)); 
    j++;
	}
	
}







/*---------------------------------------------------------------------------------------*/


/*--任务初始化task_init(sys_init()中调用)及运行函数task_run(main.c中调用)，本部分无须修改--*/

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
//系统消息处理
static uint8_t message_buff[256];
//uint8_t wifi_temp_buff[256];
char blut_Buf[512];
char Serial_Buf[512];
char Res_ValueBuf[8];
u32 Serial_BTL=0;
u8 Res_Value=0;
void msg_process(void)  //系统消息处理
{
	system_message sys_msg;	
	if(rt_ringbuffer_data_len(&msg_ring_buf)!= 0) //消息队列不为空，则有消息需要处理
	{
		rt_ringbuffer_get(&msg_ring_buf,(uint8_t *) &sys_msg.msg_type, 2);//读取消息类型(1字节) + 消息参数(1字节)，其中：不同消息类型对应的消息参数意义也不同，由用户自行定义
		
		switch(sys_msg.msg_type)
		{
			case MSG_UART1_REC_FINISH://有线
				
				  rt_ringbuffer_get(&uart1_recv_ring_buf, message_buff, sys_msg.msg_parameter);
			    message_buff[sys_msg.msg_parameter] = '\0';//sys_msg.msg_parameter中存放的是串口1接收帧的长度信息
			
			    //YouXian_Printf("uart1 recv:%s\n",message_buff);
					ChuanKouPin_Printf("serial.slt1.txt+=\"%s\r\n\"\xff\xff\xff",message_buff);
				break;
      case MSG_USART0_REC_FINISH://wifi
//				  rt_ringbuffer_get(&usart0_recv_ring_buf, message_buff, sys_msg.msg_parameter);
//			    message_buff[sys_msg.msg_parameter] = '\0';
//			    printf("usart0 recv:%s\n",message_buff);
				break;
			case MSG_UART0_REC_FINISH://串口屏
				  rt_ringbuffer_get(&uart0_recv_ring_buf, message_buff, sys_msg.msg_parameter);
			    message_buff[sys_msg.msg_parameter] = '\0';//sys_msg.msg_parameter中存放的是串口0接收帧的长度信息
					//通过串口屏来设置有线调试的波特率
					if(strstr((char*)message_buff,"BTLSET")!=NULL)
						{
							char *ptr = strstr((char*)message_buff, "BTLSET");
							ptr += strlen("BTLSET");
							sscanf(ptr, "%[^\n]", Serial_Buf);//赋值
							Serial_BTL=atoi(Serial_Buf);
							ChuanKouPin_Printf("serial.slt1.txt+=\"%s\r\n\"\xff\xff\xff",Serial_Buf);//在有线调试界面显示
							YouXian_Printf("波特率：%d",Serial_BTL);
							s_UART1_RecFrameLen=0;
							USART_TxCmd(HT_UART1, DISABLE);
							USART_RxCmd(HT_UART1, DISABLE);
							Uart1_Init(Serial_BTL);//有线调试的波特率
							USART_TxCmd(HT_UART1, ENABLE);
							USART_RxCmd(HT_UART1, ENABLE);
						}
						else if(strstr((char*)message_buff,"ResSet")!=NULL)
						{
							int Temp_Flag=0;
							char *ptr = strstr((char*)message_buff, "ResSet");
							ptr += strlen("ResSet");
							sscanf(ptr, "%[^\n]", Res_ValueBuf);//赋值
							Res_Value=atoi(Res_ValueBuf);
							SourceWriteByte(Res_Value);
							Temp_Flag=SourceWriteByte(Res_Value);
							if(Temp_Flag!=0)
							{
								//阻值设置失败，串口屏上将改变的阻值显示在对应的位置
								ChuanKouPin_Printf("power.t1.txt+=\"运行状态异常\"\xff\xff\xff");//在有线调试界面显示
							}
							else 
							{
								//阻值设置失败，串口屏上电源的运行状态为异常
								ChuanKouPin_Printf("power.t1.txt+=\"运行状态正常\"\xff\xff\xff");//在有线调试界面显示
							}
						}
				break;
					case MSG_USART1_REC_FINISH://usart1蓝牙
					rt_ringbuffer_get(&usart1_recv_ring_buf, message_buff, sys_msg.msg_parameter);
			    message_buff[sys_msg.msg_parameter] = '\0';//sys_msg.msg_parameter中存放的是串口0接收帧的长度信息
			   //YouXian_Printf("usart1 recv:%s\n",message_buff);
				break;
			default:break;
		}	
		
	}
	
}
/*---------------------------------------------------------------------------------------*/

int task_run(void)
{
	timer_loop();  //定时任务查询及执行
	msg_process(); //系统消息处理
	return 0;
}
/*---------------------------------------------------------------------------------------*/
