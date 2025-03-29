#include "toatly.h"


char *StreamDataName[7]={"VOLT","ELEC","Apparent_power","PF","POW","Temper","NUM"};


	/********************************************************************************************************
**	函数名: 	void Send_onenet(void)
**------------------------------------------------------------------------------------------------------- 
**	功能: 	发送数据到云端
**	形参 :无
**	返回值: 无
*******************************************************************************************************/
void Send_onenet(void)
{	
		char buf[250];
//	  station_int();
		Sket_Delayms(10);
	  HTTP_PostPkt_DataStreams(buf, "s52Sq=R7sHp2N9we4=RA0M6IYN0=","25448701",StreamDataName,g_val,6);//封装为多数据流同时发送，减少发送数据量，请验证！
	  WIFI_SendCmd(buf,0);	
//	   exeit();		
}
	
	/********************************************************************************************************
**	函数名: 	void Send_HIM(void)
**------------------------------------------------------------------------------------------------------- 
**	功能: 	//发送数据到屏幕
**	形参 :无
**	返回值: 无
*******************************************************************************************************/
void Send_HIM(void)
{	
	  char buf[100];
		AttGetAllVolue();
	
		sprintf(buf,"main.t%d.txt=\"%.2f\"",0,g_val[0]); //t0.txt 电压
    HMI_SendCmd(buf);
	  HMI_Sendbit(0xff);
		memset(buf,'\0',sizeof(buf));
		
		sprintf(buf,"main.t%d.txt=\"%.2f\"",1,g_val[1]); //t1.txt  电流
    HMI_SendCmd(buf);
	  HMI_Sendbit(0xff);
		memset(buf,'\0',sizeof(buf));
		

	  sprintf(buf,"main.t%d.txt=\"%.2f\"",2,g_val[2]);
    HMI_SendCmd(buf);
	  HMI_Sendbit(0xff);
		memset(buf,'\0',sizeof(buf));
	
		sprintf(buf,"main.t%d.txt=\"%.2f\"",4,g_val[3]); //t4.txt 功率因数
    HMI_SendCmd(buf);
  	HMI_Sendbit(0xff);
		
		
		sprintf(buf,"main.t%d.txt=\"%.2f\"",3,g_val[4]); //t3.txt 有功电能
    HMI_SendCmd(buf);
	  HMI_Sendbit(0xff);
		
		memset(buf,'\0',sizeof(buf));
		
	
		sprintf(buf,"main.t%d.txt=\"%.2f\"",5,g_val[5]); //t5.txt 温度
    HMI_SendCmd(buf);
	  HMI_Sendbit(0xff);
}	

void Send_key(unsigned char key)
{
	
		char buf[250];

	//	AttGetAllVolue(); //此处删除电能芯片参数读取任务，将读取电能任务单独处理，移至system.c
	  station_int();
	  HTTP_PostPkt_DataStreams_1(buf, "s52Sq=R7sHp2N9we4=RA0M6IYN0=","25448701","NUM",key);
	  WIFI_SendCmd(buf,0);
	  memset(buf,'\0',sizeof(buf));
		exeit();
}

