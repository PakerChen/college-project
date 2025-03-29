#include "config.h"
#include "bsp_ATT.h"

// ATT7022 SPI�궨��
#define		ATT_SPI_Init()				SPI1_Init()										
#define		ATT_SPI_ReadWrite(dat)		SPI1_ReadWrite(dat)	

#define ATT_SEL_H  HT_GPIOC->SRR = GPIO_PIN_1;	
#define ATT_SEL_L  HT_GPIOC->RR = GPIO_PIN_1;	
//��ѹ�����������ʡ�����ȫ�ֱ�����
double U=0;
double I=0;
double P=0;
double Q=0;
extern float g_val[9];

//-------------------------------------------------------------------------------
// ATT7022У׼����
static Energy_t s_tEmt = {0};
//-------------------------------------------------------------------------------
// ATT7022У׼����
static Revise_t s_tRts ={0xDC,{1.67,1.67,1.67},{34.3,34.3,34.3}};  


/*******************************************************************************************************
** ����: ReadData_ATT7022,  �������Ĵ���
**------------------------------------------------------------------------------------------------------
** ����: addr:�������ݵĵ�ַ(���λΪ0)	
** ����: temp:��������(24λ) 
********************************************************************************************************/
unsigned long AttReadData(unsigned char addr)
{
	unsigned long temp=0;
	//ATTCS=0;
  ATT_SEL_L;
	ATT_SPI_ReadWrite(addr&0x7f);						
	Sket_Delayus(2);// SPIʱ��Ƶ�ʹ��ߣ�������ʱ��Ҫ�ȴ�2us
	temp=(temp<<8)|ATT_SPI_ReadWrite(0);
	temp=(temp<<8)|ATT_SPI_ReadWrite(0);
	temp=(temp<<8)|ATT_SPI_ReadWrite(0);
	ATT_SEL_H;	
	return temp;   							 		
}

/*******************************************************************************************************
** ����: WriteData_ATT7022,  д�������У������
**------------------------------------------------------------------------------------------------------
** ����: addr:��д���ݵĵ�ַ dat ��д������
** ����: void 
********************************************************************************************************/
void WriteData_ATT7022(unsigned char addr,unsigned long dat)
{	  
	//ATTCS=0;
	ATT_SEL_L;
	ATT_SPI_ReadWrite(addr|0x80);
	ATT_SPI_ReadWrite(dat>>16);
	ATT_SPI_ReadWrite(dat>>8);
	ATT_SPI_ReadWrite(dat>>0);
	//ATTCS=1;
	ATT_SEL_H;
}
/*******************************************************************************************************
** ����: AttVarInit, ATT7022�Ĵ�������
**------------------------------------------------------------------------------------------------------
** ����: NOLL
** ����: NOLL
********************************************************************************************************/
void ATT7022VarInit(void)
{	
	WriteData_ATT7022( 0xC3,0x000000); 							// ��У������
	WriteData_ATT7022( 0xC9,0x00005A); 							// ��У׼����д
	WriteData_ATT7022( 0x01,0xB97E);							// ģʽ���üĴ���
	WriteData_ATT7022( 0x03,0xF804);							// EMU ��Ԫ���üĴ���
	WriteData_ATT7022( 0x31,0x3427);							// ģ��ģ��ʹ�ܼĴ���
	WriteData_ATT7022( 0x6D,0xFF00); 							// Vrefgain �Ĳ�������ϵ��
	WriteData_ATT7022( 0x6E,0x0DB8); 							// Vrefgain �Ĳ�������ϵ��
	WriteData_ATT7022( 0x6F,0xD1DA); 							// Vrefgain �Ĳ�������ϵ��
	WriteData_ATT7022( 0x16,0x0000);							// �޹���λУ���Ĵ���
	WriteData_ATT7022( 0x1E,HFconst);							// HFConst��Ƶ�������У׼
	WriteData_ATT7022( 0xC9,0x0001); 							// �ر�У׼����д
//	printf("%x\n\n",(u32)AttReadData(0));							// ����ָ�� ��ӡ 7122A0 оƬ���� SPIͨ������
}
/********************************************************************************************************
** ����: Att7022Init
**------------------------------------------------------------------------------------------------------
** ����: void
** ����: void
*******************************************************************************************************/
void Att7022Init(void)
{
	ATT_SPI_Init();
	ATT7022VarInit();
	AttGetAllVolue();
}
/*******************************************************************************************************
** ����: AttGetVolue 
**------------------------------------------------------------------------------------------------------
** ����: addr ��ַ 
** ����: �з�������
********************************************************************************************************/
long AttGetVolue( unsigned char Value)
{	
	long temp;
	temp=AttReadData(Value);
	if(AttReadData(0X2d)!=temp) return -1;
	if(temp&0x800000) temp = 0x1000000 - temp ;  
	return temp;
}
/*******************************************************************************************************
** ����: AttGetUnVolue 
**------------------------------------------------------------------------------------------------------
** ����: addr ��ַ 
** ����: �޷�������
********************************************************************************************************/
long AttGetUnVolue(  unsigned char Value)
{	
	long temp;
	temp=AttReadData(Value);
	if(AttReadData(0X2d)!=temp) return -1;
	return 	temp;
}
/*******************************************************************************************************
** ����: AttGetAllVolue, ��ȡATT7022UE�ڲ����ܼ���ֵ
**------------------------------------------------------------------------------------------------------
** ����: Energy_t *dat
** ����: NONE
********************************************************************************************************/
	void AttGetAllVolue( void )
{	
	//----------------------------------------------------------------------------------------------- 
	// A����ܲ����ɼ�
	s_tEmt.dMU [ItemA]   = AttGetVolue(VOLTAGE+ItemA)/UKrmsA;				   		// A���ѹ
	g_val[0]=s_tEmt.dMU [ItemA];
	s_tEmt.dMI [ItemA]   = AttGetVolue(CURRENT+ItemA)/IKrmsA;				  		// A�����
		g_val[1]=s_tEmt.dMI [ItemA];
	s_tEmt.dMP [ItemA]   = AttGetVolue(POWERS+ItemA)*KCoefficient;				// A���й�����
		g_val[2]=s_tEmt.dMP [ItemA];
	s_tEmt.dMQ [ItemA]   = AttGetVolue(QPOWERS+ItemA)*KCoefficient;		   		// A���޹�����
	s_tEmt.dMPF[ItemA]   = AttGetVolue(PF+ItemA)/8388608.0;				   		// A�๦������
	g_val[3]=s_tEmt.dMPF[ItemA];
	s_tEmt.dwMPoEpt[ItemA] = AttGetUnVolue(EP+ItemA);	          	// A���й�����
	g_val[4]=s_tEmt.dwMPoEpt[ItemA];
	s_tEmt.dwMQoEpt[ItemA] = AttGetUnVolue(EQ+ItemA);								// A���޹�����
	
	//----------------------------------------------------------------------------------------------- 
	// B����ܲ����ɼ�
	s_tEmt.dMU [ItemB]   = AttGetVolue(VOLTAGE+ItemB)/UKrmsB;				   		// B���ѹ
	//g_val[4]=s_tEmt.dMU [ItemB];
	s_tEmt.dMI [ItemB]   = AttGetVolue(CURRENT+ItemB)/IKrmsB;			  			// B�����
	//	g_val[1]=s_tEmt.dMI [ItemB];
	s_tEmt.dMP [ItemB]   = AttGetVolue(POWERS+ItemB)*KCoefficient;				// B���й�����
//			g_val[7]=s_tEmt.dMP [ItemB];
	s_tEmt.dMQ [ItemB]   = AttGetVolue(QPOWERS+ItemB)*KCoefficient;		   		// B���޹�����
	s_tEmt.dMPF[ItemB]   = AttGetVolue(PF+ItemB)/8388608.0;				   		// B�๦������
	s_tEmt.dwMPoEpt[ItemB] = AttGetUnVolue(EP+ItemB);								// B���й�����
	s_tEmt.dwMQoEpt[ItemB] = AttGetUnVolue(EQ+ItemB);								// B���޹�����
	
	//----------------------------------------------------------------------------------------------- 
	// C����ܲ����ɼ�
	s_tEmt.dMU [ItemC]   = AttGetVolue(VOLTAGE+ItemC)/UKrmsC;					   	// C���ѹ	
//	g_val[5]=s_tEmt.dMU [ItemC];
	s_tEmt.dMI [ItemC]   = AttGetVolue(CURRENT+ItemC)/IKrmsC;					  	// C�����	
	//	g_val[2]=s_tEmt.dMI [ItemC];
	s_tEmt.dMP [ItemC]   = AttGetVolue(POWERS+ItemC)*KCoefficient;				// C���й�����
	//		g_val[8]=s_tEmt.dMP [ItemC];
	s_tEmt.dMQ [ItemC]   = AttGetVolue(QPOWERS+ItemC)*KCoefficient;				// C���޹�����
	s_tEmt.dMPF[ItemC]   = AttGetVolue(PF+ItemC)/8388608.0;						// C�๦������
	s_tEmt.dwMPoEpt[ItemC] = AttGetUnVolue(EP+ItemC);								// C���й�����
	s_tEmt.dwMQoEpt[ItemC] = AttGetUnVolue(EQ+ItemC);								// C���޹�����
	
	//----------------------------------------------------------------------------------------------- 
	// �ܵ��ܲ����ɼ�
	s_tEmt.dMF  = AttGetVolue(FREQUENCY)/8191.0;									// ��Ƶ��
	s_tEmt.dMP [ItemALL]   = AttGetVolue(POWERS+ItemALL)*KCoefficient*2;			// ���й�����
	s_tEmt.dMQ [ItemALL]   = AttGetVolue(QPOWERS+ItemALL)*KCoefficient*2;			// ���޹�����
 	s_tEmt.dMPF[ItemALL] = AttGetVolue(PF+ItemALL)/8388608.0;						// �ܹ�������
	s_tEmt.dwMPoEpt[ItemALL] = AttGetUnVolue(EP+ItemALL);							// ���й�����
	s_tEmt.dwMQoEpt[ItemALL] = AttGetUnVolue(EQ+ItemALL);							// ���޹�����
}

