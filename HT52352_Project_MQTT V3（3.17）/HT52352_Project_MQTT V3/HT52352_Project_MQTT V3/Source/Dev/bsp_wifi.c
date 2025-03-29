/*-------------------------------------------------*/
/*            ��γ����STM32ϵ�п�����              */
/*-------------------------------------------------*/
/*                                                 */
/*            ����602Wifi���ܵ�Դ�ļ�              */
/*                                                 */
/*-------------------------------------------------*/

#include "config.h"  //������Ҫ��ͷ�ļ�
#include "bsp_wifi.h"	    //������Ҫ��ͷ�ļ�


char wifi_mode = 0;     //����ģʽ 0��SSID������д�ڳ�����   1��Smartconfig��ʽ��APP����
	
/*-------------------------------------------------*/
/*����������ʼ��WiFi�ĸ�λIO  RST--PA0              */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void WiFi_ResetIO_Init(void)
{
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
	
  CKCUClock.Bit.PA = 1;
	CKCUClock.Bit.AFIO  = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);
	                                             
	/* Configure AFIO mode of output pins                                                                     */
	AFIO_GPxConfig(GPIO_PA, GPIO_PIN_0, AFIO_FUN_GPIO);

	/* Configure GPIO direction of output pins                                                                */
	GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_0, GPIO_DIR_OUT);

  GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_0, SET);
}
/*-------------------------------------------------*/
/*��������WiFi��������ָ��                         */
/*��  ����cmd��ָ��                                */
/*��  ����timeout����ʱʱ�䣨100ms�ı�����         */
/*����ֵ��0����ȷ   ����������                     */
/*-------------------------------------------------*/
char WiFi_SendCmd(char *cmd, int timeout)
{
	WiFi_RxCounter=0;                           //WiFi������������������                        
	memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);     //���WiFi���ջ����� 
	WiFi_printf("%s\r\n",cmd);                  //����ָ��
	while(timeout--)
	{                           //�ȴ���ʱʱ�䵽0
		SysTickDelayMs(100);                          //��ʱ100ms
		if(strstr(WiFi_RX_BUF,"OK"))            //������յ�OK��ʾָ��ɹ�
			break;       						//��������whileѭ��
		printf("%d ",timeout);               //����������ڵĳ�ʱʱ��
		UART0_printf("wifi.slt1.txt+=\"%d\r\"\xff\xff\xff",timeout);
	}
	printf("\r\n");                          //���������Ϣ
	if(timeout<=0)return 1;                     //���timeout<=0��˵����ʱʱ�䵽�ˣ�Ҳû���յ�OK������1
	else return 0;		         				//��֮����ʾ��ȷ��˵���յ�OK��ͨ��break��������while
}
/*-------------------------------------------------*/
/*��������WiFi��λ                                 */
/*��  ����timeout����ʱʱ�䣨100ms�ı�����         */
/*����ֵ��0����ȷ   ����������                     */
/*-------------------------------------------------*/
char WiFi_Reset(int timeout)
{
	RESET_IO(0);                                    //��λIO���͵�ƽ
	SysTickDelayMs(1000);                                  //��ʱ500ms
	RESET_IO(1);                                    //��λIO���ߵ�ƽ	
	while(timeout--){                               //�ȴ���ʱʱ�䵽0
		SysTickDelayMs(100);                              //��ʱ100ms
		
		if(strstr(WiFi_RX_BUF,"ready"))             //������յ�ready��ʾ��λ�ɹ�
			break;       						    //��������whileѭ��
		printf("%d ",timeout);                   //����������ڵĳ�ʱʱ��
	}
	printf("\r\n");                              //���������Ϣ
	if(timeout<=0)return 1;                         //���timeout<=0��˵����ʱʱ�䵽�ˣ�Ҳû���յ�ready������1
	else return 0;		         				    //��֮����ʾ��ȷ��˵���յ�ready��ͨ��break��������while
}
/*-------------------------------------------------*/
/*��������WiFi����·����ָ��                       */
/*��  ����timeout����ʱʱ�䣨1s�ı�����            */
/*����ֵ��0����ȷ   ����������                     */
/*-------------------------------------------------*/
char WiFi_JoinAP(int timeout)
{		
	WiFi_RxCounter=0;                               //WiFi������������������                        
	memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);         //���WiFi���ջ����� 
	WiFi_printf("AT+CWJAP=\"%s\",\"%s\"\r\n",SSID,PASS); //����ָ��	
	while(timeout--){                               //�ȴ���ʱʱ�䵽0
		SysTickDelayMs(1000);                             //��ʱ1s
		if(strstr(WiFi_RX_BUF,"WIFI GOT IP\r\n\r\nOK")) //������յ�WIFI GOT IP��ʾ�ɹ�
			break;       						    //��������whileѭ��
		printf("%d ",timeout);                   //����������ڵĳ�ʱʱ��
		UART0_printf("wifi.slt1.txt+=\"%d\r\"\xff\xff\xff",timeout);	
	}
	printf("\r\n");                              //���������Ϣ
	UART0_printf("wifi.slt1.txt+=\"\r\n\"\xff\xff\xff");
	if(timeout<=0)return 1;                         //���timeout<=0��˵����ʱʱ�䵽�ˣ�Ҳû���յ�WIFI GOT IP������1
	return 0;                                       //��ȷ������0
}
/*-------------------------------------------------*/
/*������������TCP��������������͸��ģʽ            */
/*��  ����timeout�� ��ʱʱ�䣨100ms�ı�����        */
/*����ֵ��0����ȷ  ����������                      */
/*-------------------------------------------------*/
char WiFi_Connect_Server(int timeout)
{	
	WiFi_RxCounter=0;                               //WiFi������������������                        
	memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);         //���WiFi���ջ�����   
	WiFi_printf("AT+CIPSTART=\"TCP\",\"%s\",%d\r\n",ServerIP,ServerPort);//�������ӷ�����ָ��
	while(timeout--){                               //�ȴ���ʱ���
		SysTickDelayMs(100);                              //��ʱ100ms	
		if(strstr(WiFi_RX_BUF ,"CONNECT"))          //������ܵ�CONNECT��ʾ���ӳɹ�
			break;                                  //����whileѭ��
		if(strstr(WiFi_RX_BUF ,"CLOSED"))           //������ܵ�CLOSED��ʾ������δ����
			return 1;                               //������δ��������1
		if(strstr(WiFi_RX_BUF ,"ALREADY CONNECTED"))//������ܵ�ALREADY CONNECTED�Ѿ���������
			return 2;                               //�Ѿ��������ӷ���2
		printf("%d ",timeout);                   //����������ڵĳ�ʱʱ��  
	}
	printf("\r\n");                        //���������Ϣ
	if(timeout<=0)return 3;                   //��ʱ���󣬷���3
	else                                      //���ӳɹ���׼������͸��
	{
		printf("���ӷ������ɹ���׼������͸��\r\n");  //������ʾ��Ϣ
		WiFi_RxCounter=0;                               //WiFi������������������                        
		memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);         //���WiFi���ջ�����     
		WiFi_printf("AT+CIPSEND\r\n");                  //���ͽ���͸��ָ��
		while(timeout--){                               //�ȴ���ʱ���
			SysTickDelayMs(100);                              //��ʱ100ms	
			if(strstr(WiFi_RX_BUF,"\r\nOK\r\n\r\n>"))   //���������ʾ����͸���ɹ�
				break;                          //����whileѭ��
			printf("%d ",timeout);           //����������ڵĳ�ʱʱ��  
		}
		if(timeout<=0)return 4;                 //͸����ʱ���󣬷���4	
	}
	return 0;	                                //�ɹ�����0	
}
/*-------------------------------------------------*/
/*��������WiFi_Smartconfig                         */
/*��  ����timeout����ʱʱ�䣨1s�ı�����            */
/*����ֵ��0����ȷ   ����������                     */
/*-------------------------------------------------*/
char WiFi_Smartconfig(int timeout)
{
	
	WiFi_RxCounter=0;                           //WiFi������������������                        
	memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);     //���WiFi���ջ�����     
	while(timeout--){                           //�ȴ���ʱʱ�䵽0
		SysTickDelayMs(1000);                         //��ʱ1s
		if(strstr(WiFi_RX_BUF,"connected"))     //������ڽ��ܵ�connected��ʾ�ɹ�
			break;                              //����whileѭ��  
		printf("%d ",timeout);               //����������ڵĳ�ʱʱ��  
	}	
	printf("\r\n");                          //���������Ϣ
	if(timeout<=0)return 1;                     //��ʱ���󣬷���1
	return 0;                                   //��ȷ����0
}
/*-------------------------------------------------*/
/*���������ȴ�����·����                           */
/*��  ����timeout����ʱʱ�䣨1s�ı�����            */
/*����ֵ��0����ȷ   ����������                     */
/*-------------------------------------------------*/
char WiFi_WaitAP(int timeout)
{		
	while(timeout--){                               //�ȴ���ʱʱ�䵽0
		SysTickDelayMs(1000);                             //��ʱ1s
		if(strstr(WiFi_RX_BUF,"WIFI GOT IP"))       //������յ�WIFI GOT IP��ʾ�ɹ�
			break;       						    //��������whileѭ��
		printf("%d ",timeout);                   //����������ڵĳ�ʱʱ��
	}
	printf("\r\n");                              //���������Ϣ
	if(timeout<=0)return 1;                         //���timeout<=0��˵����ʱʱ�䵽�ˣ�Ҳû���յ�WIFI GOT IP������1
	return 0;                                       //��ȷ������0
}
/*-------------------------------------------------*/
/*��������WiFi���ӷ�����                           */
/*��  ������                                       */
/*����ֵ��0����ȷ   ����������                     */
/*-------------------------------------------------*/
char WiFi_Connect_IoTServer(void)
{	

	printf("׼�����Ӱ����Ʒ�����\r\n");                   //������ʾ����
	if(WiFi_Connect_Server(100)){                      //���ӷ�������100ms��ʱ��λ���ܼ�10s��ʱʱ��
		printf("���ӷ�����ʧ�ܣ�׼������\r\n");     //���ط�0ֵ������if��������ʾ����
		return 10;                                     //����10
	}else printf("���ӷ������ɹ�\r\n");             //������ʾ����	
	
	return 0;                                          //��ȷ����0
	
}
//WiFi_ATָ����ԣ��������û�����⣬����0�����򷵻�1
u8 WiFi_AT_Test ( void )
{
	char count=0;
	RESET_IO(1);                                    //��λIO���ߵ�ƽ	
	SysTickDelayMs(100);   
	while ( count < 10 )
	{
//		printf("��%d�β���ATָ��\r\n",count+1); 
		UART0_printf("wifi.slt1.txt+=\"��%d�β���ATָ��\r\n\"\xff\xff\xff",count+1);		
		if(WiFi_SendCmd("AT",50)==0) return 0;
//		printf("��%d�β���ATָ��ʧ�ܣ�׼���Զ����븴λ����һ�β���\r\n",count+1);
		UART0_printf("wifi.slt1.txt+=\"��%d�β���ATָ��ʧ�ܣ�׼���Զ����븴λ����һ�β���\r\n\"\xff\xff\xff",count+1);
		RESET_IO(0);                                    //��λIO���͵�ƽ
	  SysTickDelayMs(1000);                                  //��ʱ500ms
	  RESET_IO(1);                                    //��λIO���ߵ�ƽ	
		SysTickDelayMs(1000);   
		++ count;
		
	}
	return 1;
}
u8 Wifi_connect_flag=0;
char WiFi_Connect_Init(void)
{
		//if(Wifi_connect_flag==1)
		//{
		//	Wifi_connect_flag++;
//		printf("׼������WIFIģ��\r\n");
		UART0_printf("wifi.slt1.txt+=\"׼������WIFIģ��\r\n\"\xff\xff\xff");			
		Usart0_Init(115200);                             //����0���ܳ�ʼ����������115200	
		WiFi_RxCounter=0;                                //WiFi������������������                        
		memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);          //���WiFi���ջ����� 
	
//	printf("׼����λWIFIģ��\r\n");                     //������ʾ����

//	if(WiFi_Reset(50)){                                //��λ��100ms��ʱ��λ���ܼ�5s��ʱʱ��
//		printf("WIFIģ�鸴λʧ�ܣ�׼������\r\n");           //���ط�0ֵ������if��������ʾ����
//		return 1;                                      //����1
//	}else printf("WIFIģ�鸴λ�ɹ�\r\n");                   //������ʾ����
//	
//	printf("WIFIģ��ATָ�����\r\n"); 
	UART0_printf("wifi.slt1.txt+=\"WIFIģ��ATָ�����\r\n\"\xff\xff\xff");
	if(WiFi_AT_Test())
	{ 
//		printf("WIFIģ��ATָ�����ʧ�ܣ�׼������\r\n");   
		UART0_printf("wifi.slt1.txt+=\"WIFIģ��ATָ�����ʧ�ܣ�׼������\r\n\"\xff\xff\xff");
	  return 1;
	}
	else 
		{
//			printf("WIFIģ��ATָ����Գɹ�\r\n");
			UART0_printf("wifi.slt1.txt+=\"WIFIģ��ATָ����Գɹ�\r\n\"\xff\xff\xff");
		}
	
//	printf("׼������STAģʽ\r\n");		//������ʾ����
	UART0_printf("wifi.slt1.txt+=\"׼������STAģʽ\r\n\"\xff\xff\xff");
	if(WiFi_SendCmd("AT+CWMODE=1",50)){                //����STAģʽ��100ms��ʱ��λ���ܼ�5s��ʱʱ��
//		printf("����STAģʽʧ�ܣ�׼������\r\n"); 		//���ط�0ֵ������if��������ʾ����
		UART0_printf("wifi.slt1.txt+=\"����STAģʽʧ�ܣ�׼������\r\n\"\xff\xff\xff");
		return 2;                                      //����2
	}
	else 
	{
//		printf("����STAģʽ�ɹ�\r\n");            //������ʾ����
		UART0_printf("wifi.slt1.txt+=\"����STAģʽ�ɹ�\r\n\"\xff\xff\xff");
	}
		
	if(wifi_mode==0)
	{                                      //�������ģʽ=0��SSID������д�ڳ����� 
//		printf("׼��ȡ���Զ�����\r\n");                 //������ʾ����
		UART0_printf("wifi.slt1.txt+=\"׼��ȡ���Զ�����\r\n\"\xff\xff\xff");
		if(WiFi_SendCmd("AT+CWAUTOCONN=0",50)){            //ȡ���Զ����ӣ�100ms��ʱ��λ���ܼ�5s��ʱʱ��
//			printf("ȡ���Զ�����ʧ�ܣ�׼������\r\n");   //���ط�0ֵ������if��������ʾ����
			UART0_printf("wifi.slt1.txt+=\"ȡ���Զ�����ʧ�ܣ�׼������\r\n\"\xff\xff\xff");
			return 3;                                      //����3
		}
		else 
		{
//			printf("ȡ���Զ����ӳɹ�\r\n");           //������ʾ����
			UART0_printf("wifi.slt1.txt+=\"ȡ���Զ����ӳɹ�\r\n\"\xff\xff\xff");
		}				
//		printf("׼������·����\r\n");                   //������ʾ����	
		UART0_printf("wifi.slt1.txt+=\"׼������·����\r\n\"\xff\xff\xff");
		if(WiFi_JoinAP(30)){                               //����·����,1s��ʱ��λ���ܼ�30s��ʱʱ��
//			printf("����·����ʧ�ܣ�׼������\r\n");     //���ط�0ֵ������if��������ʾ����
			UART0_printf("wifi.slt1.txt+=\"����·����ʧ�ܣ�׼������\r\n\"\xff\xff\xff");
			return 4;                                      //����4	
		}else 
		{
//			printf("����·�����ɹ�\r\n");             //������ʾ����			
			UART0_printf("wifi.slt1.txt+=\"����·�����ɹ�\r\n\"\xff\xff\xff");
		}
	}
	else
	{                                                 //�������ģʽ=1��Smartconfig��ʽ,��APP����
//		if(Key_Scan()==Key0_Value){                                    //�����ʱK2�ǰ��µ�
//			printf("׼�������Զ�����\r\n");                 //������ʾ����
//			if(WiFi_SendCmd("AT+CWAUTOCONN=1",50)){            //�����Զ����ӣ�100ms��ʱ��λ���ܼ�5s��ʱʱ��
//				printf("�����Զ�����ʧ�ܣ�׼������\r\n");   //���ط�0ֵ������if��������ʾ����
//				return 3;                                      //����3
//			}else printf("�����Զ����ӳɹ�\r\n");           //������ʾ����	
//			
//			printf("׼������Smartconfig\r\n");              //������ʾ����
//			if(WiFi_SendCmd("AT+CWSTARTSMART",50)){            //����Smartconfig��100ms��ʱ��λ���ܼ�5s��ʱʱ��
//				printf("����Smartconfigʧ�ܣ�׼������\r\n");//���ط�0ֵ������if��������ʾ����
//				return 4;                                      //����4
//			}else printf("����Smartconfig�ɹ�\r\n");        //������ʾ����

//			printf("��ʹ��APP�����������\r\n");            //������ʾ����
//			if(WiFi_Smartconfig(60)){                          //APP����������룬1s��ʱ��λ���ܼ�60s��ʱʱ��
//				printf("��������ʧ�ܣ�׼������\r\n");       //���ط�0ֵ������if��������ʾ����
//				return 5;                                      //����5
//			}else printf("��������ɹ�\r\n");               //������ʾ����

//			printf("׼���ر�Smartconfig\r\n");              //������ʾ����
//			if(WiFi_SendCmd("AT+CWSTOPSMART",50)){             //�ر�Smartconfig��100ms��ʱ��λ���ܼ�5s��ʱʱ��
//				printf("�ر�Smartconfigʧ�ܣ�׼������\r\n");//���ط�0ֵ������if��������ʾ����
//				return 6;                                      //����6
//			}else printf("�ر�Smartconfig�ɹ�\r\n");        //������ʾ����
//		}else{                                                 //��֮����ʱK2��û�а���
//			printf("�ȴ�����·����\r\n");                   //������ʾ����	
//			if(WiFi_WaitAP(30)){                               //�ȴ�����·����,1s��ʱ��λ���ܼ�30s��ʱʱ��
//				printf("����·����ʧ�ܣ�׼������\r\n");     //���ط�0ֵ������if��������ʾ����
//				return 7;                                      //����7	
//			}else printf("����·�����ɹ�\r\n");             //������ʾ����					
//		}
	}
	
//	printf("׼������͸��\r\n");                     //������ʾ����
	UART0_printf("wifi.slt1.txt+=\"׼������͸��\r\n\"\xff\xff\xff");
	if(WiFi_SendCmd("AT+CIPMODE=1",50)){               //����͸����100ms��ʱ��λ���ܼ�5s��ʱʱ��
//		printf("����͸��ʧ�ܣ�׼������\r\n");       //���ط�0ֵ������if��������ʾ����
		UART0_printf("wifi.slt1.txt+=\"����͸��ʧ�ܣ�׼������\r\n\"\xff\xff\xff");
		return 8;                                      //����8
	}else
{
//	printf("����͸���ɹ�\r\n");               //������ʾ����
	UART0_printf("wifi.slt1.txt+=\"����͸���ɹ�\r\n\"\xff\xff\xff");
}
	
//	printf("׼���رն�·����\r\n");                 //������ʾ����
	UART0_printf("wifi.slt1.txt+=\"׼���رն�·����\r\n\"\xff\xff\xff");
	if(WiFi_SendCmd("AT+CIPMUX=0",50)){                //�رն�·���ӣ�100ms��ʱ��λ���ܼ�5s��ʱʱ��
//		printf("�رն�·����ʧ�ܣ�׼������\r\n");   //���ط�0ֵ������if��������ʾ����
		UART0_printf("wifi.slt1.txt+=\"�رն�·����ʧ�ܣ�׼������\r\n\"\xff\xff\xff");
		return 9;                                      //����9
	}else
	{	
//		printf("�رն�·���ӳɹ�\r\n");           //������ʾ����
		UART0_printf("wifi.slt1.txt+=\"�رն�·���ӳɹ�\r\n\"\xff\xff\xff");
	}
	
	
		if(WiFi_Connect_IoTServer()==0){   			     //���WiFi�����Ʒ�������������0����ʾ���ӳɹ�������if
//		printf("���ӷ������ɹ�\r\n");              //���������Ϣ
		UART0_printf("wifi.slt1.txt+=\"���ӷ������ɹ�\r\n\"\xff\xff\xff");
		Usart0_IDELInit(115200);                     //����2 ����DMA �� �����ж�
		Connect_flag = 1;                            //Connect_flag��1����ʾ���ӳɹ�					
		WiFi_RxCounter=0;                            //WiFi������������������                        
		memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);      //���WiFi���ջ����� 
		
	  }
	//}
		return 0;
}
		

