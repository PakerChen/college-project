#ifndef _INA226_H
#define _INA226_H

#include "config.h"

#define 	CFG_REG	 		0x00		//���üĴ�����ַ
#define 	SV_REG 			0x01		//������ѹ�Ĵ�����ַ
#define 	BV_REG 			0x02		//���ߵ�ѹ
#define 	PWR_REG 		0x03		//��Դ����
#define 	CUR_REG 		0x04		//����
#define 	CAL_REG 		0x05		//У׼
#define 	ONFF_REG 		0x06		//���� 
#define 	AL_REG 			0x07		//��������ѡ����������Ƚϵ��޶�ֵ
#define 	INA226_GET_ADDR 0XFF		//����Ψһ��оƬ��ʶ��
#define   	INA226_ADDR1	0x80       //�ӻ�д��ַ


//��ʼ��
void INA226_Init(void);
//�������ݵ�reg�Ĵ���
void INA226_SendData(uint8_t reg,uint16_t data);
//��ȡ�Ĵ���reg��ֵ
uint16_t INA226_ReadData(uint8_t reg);
 
 
//��ȡУ׼ֵ
uint16_t  INA226_GetCAL(void);
//��ȡ����ֵ
uint16_t  INA226_GetCFG(void);
//��ȡоƬID
uint16_t  INA226_GetID(void);
//��ȡ���ߵ�ѹ�Ĵ�����ֵ��ֻ�ǼĴ���ֵ��������ʵ��ѹֵ��
uint16_t INA226_GetBV(void);
//��ȡ������ѹ�Ĵ�����ֵ��ֻ�ǼĴ���ֵ��������ʵ��ѹֵ��
uint16_t INA226_GetSV(void);
//��ȡ�����Ĵ�����ֵ��ֻ�ǼĴ���ֵ��������ʵ��ѹֵ��
uint16_t INA226_GetSC(void);

#endif
