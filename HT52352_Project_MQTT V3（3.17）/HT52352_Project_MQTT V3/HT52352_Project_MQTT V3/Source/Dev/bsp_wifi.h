/*-------------------------------------------------*/
/*            超纬电子STM32系列开发板              */
/*-------------------------------------------------*/
/*                                                 */
/*              操作Wifi功能的头文件               */
/*                                                 */
/*-------------------------------------------------*/

#ifndef __BSP_WIFI_H
#define __BSP_WIFI_H

#include "bsp_usart0.h"	    //包含需要的头文件



#define RESET_IO(x)    GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_0, (FlagStatus)x)  //PA1控制WiFi的复位


#define WiFi_printf       u0_printf           //串口2控制 WiFi
#define WiFi_RxCounter    Usart0_RxCounter    //串口2控制 WiFi
#define WiFi_RX_BUF       Usart0_RxBuff       //串口2控制 WiFi
#define WiFi_RXBUFF_SIZE  USART0_RXBUFF_SIZE  //串口2控制 WiFi
extern u8 Wifi_connect_flag;

#define SSID   "Esp01s"                     //热点（路由器）SSID名称
#define PASS   "12345678"                 //热点（路由器）密码

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


