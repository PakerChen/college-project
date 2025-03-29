/*-------------------------------------------------*/
/*            ��γ����STM32ϵ�п�����              */
/*-------------------------------------------------*/
/*                                                 */
/*              ����Wifi���ܵ�ͷ�ļ�               */
/*                                                 */
/*-------------------------------------------------*/

#ifndef __BSP_WIFI_H
#define __BSP_WIFI_H

#include "bsp_usart0.h"	    //������Ҫ��ͷ�ļ�



#define RESET_IO(x)    GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_0, (FlagStatus)x)  //PA1����WiFi�ĸ�λ


#define WiFi_printf       u0_printf           //����2���� WiFi
#define WiFi_RxCounter    Usart0_RxCounter    //����2���� WiFi
#define WiFi_RX_BUF       Usart0_RxBuff       //����2���� WiFi
#define WiFi_RXBUFF_SIZE  USART0_RXBUFF_SIZE  //����2���� WiFi
extern u8 Wifi_connect_flag;

#define SSID   "Esp01s"                     //�ȵ㣨·������SSID����
#define PASS   "12345678"                 //�ȵ㣨·����������

void WiFi_ResetIO_Init(void);
char WiFi_SendCmd(char *cmd, int timeout);
char WiFi_Reset(int timeout);
char WiFi_JoinAP(int timeout);
char WiFi_Connect_Server(int timeout);
char WiFi_Smartconfig(int timeout);
char WiFi_WaitAP(int timeout);
char WiFi_Connect_IoTServer(void);
char WiFi_Connect_Init(void);

#endif


