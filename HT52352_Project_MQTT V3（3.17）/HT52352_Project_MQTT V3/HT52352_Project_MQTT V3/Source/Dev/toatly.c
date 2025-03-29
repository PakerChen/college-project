#include "toatly.h"


char *StreamDataName[7]={"VOLT","ELEC","Apparent_power","PF","POW","Temper","NUM"};


	/********************************************************************************************************
**	������: 	void Send_onenet(void)
**------------------------------------------------------------------------------------------------------- 
**	����: 	�������ݵ��ƶ�
**	�β� :��
**	����ֵ: ��
*******************************************************************************************************/
void Send_onenet(void)
{	
		char buf[250];
//	  station_int();
		Sket_Delayms(10);
	  HTTP_PostPkt_DataStreams(buf, "s52Sq=R7sHp2N9we4=RA0M6IYN0=","25448701",StreamDataName,g_val,6);//��װΪ��������ͬʱ���ͣ����ٷ���������������֤��
	  WIFI_SendCmd(buf,0);	
//	   exeit();		
}
	
	/********************************************************************************************************
**	������: 	void Send_HIM(void)
**------------------------------------------------------------------------------------------------------- 
**	����: 	//�������ݵ���Ļ
**	�β� :��
**	����ֵ: ��
*******************************************************************************************************/
void Send_HIM(void)
{	
	  char buf[100];
		AttGetAllVolue();
	
		sprintf(buf,"main.t%d.txt=\"%.2f\"",0,g_val[0]); //t0.txt ��ѹ
    HMI_SendCmd(buf);
	  HMI_Sendbit(0xff);
		memset(buf,'\0',sizeof(buf));
		
		sprintf(buf,"main.t%d.txt=\"%.2f\"",1,g_val[1]); //t1.txt  ����
    HMI_SendCmd(buf);
	  HMI_Sendbit(0xff);
		memset(buf,'\0',sizeof(buf));
		

	  sprintf(buf,"main.t%d.txt=\"%.2f\"",2,g_val[2]);
    HMI_SendCmd(buf);
	  HMI_Sendbit(0xff);
		memset(buf,'\0',sizeof(buf));
	
		sprintf(buf,"main.t%d.txt=\"%.2f\"",4,g_val[3]); //t4.txt ��������
    HMI_SendCmd(buf);
  	HMI_Sendbit(0xff);
		
		
		sprintf(buf,"main.t%d.txt=\"%.2f\"",3,g_val[4]); //t3.txt �й�����
    HMI_SendCmd(buf);
	  HMI_Sendbit(0xff);
		
		memset(buf,'\0',sizeof(buf));
		
	
		sprintf(buf,"main.t%d.txt=\"%.2f\"",5,g_val[5]); //t5.txt �¶�
    HMI_SendCmd(buf);
	  HMI_Sendbit(0xff);
}	

void Send_key(unsigned char key)
{
	
		char buf[250];

	//	AttGetAllVolue(); //�˴�ɾ������оƬ������ȡ���񣬽���ȡ�������񵥶���������system.c
	  station_int();
	  HTTP_PostPkt_DataStreams_1(buf, "s52Sq=R7sHp2N9we4=RA0M6IYN0=","25448701","NUM",key);
	  WIFI_SendCmd(buf,0);
	  memset(buf,'\0',sizeof(buf));
		exeit();
}

