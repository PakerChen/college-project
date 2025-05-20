#include "spi_flash.h"
 
 
u16 W25QXX_TYPE = W25Q64;
 
void delay_us(u16 nus)
{
	u16 i;
	for (i=0; i<nus; i++)
	{
		
	}
}
 
 
 
void W25QXX_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
    FLASH_SPI_CS_APBxClock_FUN(FLASH_SPI_CS_CLK|FLASH_SPI_SCK_CLK|
							FLASH_SPI_MISO_PIN|FLASH_SPI_MOSI_PIN, ENABLE);
	
	
	/* ����SPI�� CS���ţ���ͨIO���� */
	GPIO_InitStructure.GPIO_Pin = FLASH_SPI_CS_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(FLASH_SPI_CS_PORT, &GPIO_InitStructure);
 
	/* ����SPI�� SCK����*/
	GPIO_InitStructure.GPIO_Pin = FLASH_SPI_SCK_PIN;
	GPIO_Init(FLASH_SPI_SCK_PORT, &GPIO_InitStructure);
	
	/* ����SPI�� MOSI����*/
	GPIO_InitStructure.GPIO_Pin = FLASH_SPI_MOSI_PIN;
	GPIO_Init(FLASH_SPI_MOSI_PORT, &GPIO_InitStructure);
 
 
	/* ����SPI�� MISO����*/
	GPIO_InitStructure.GPIO_Pin = FLASH_SPI_MISO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(FLASH_SPI_MISO_PORT, &GPIO_InitStructure);
 
	
	/* ֹͣ�ź� FLASH: CS���Ÿߵ�ƽ*/
	SPI_FLASH_CS_HIGH();
	
	W25QXX_TYPE = W25QXX_ReadID();
//	if (W25QXX_TYPE == W25Q64)
//	{
//		printf("FlashID: %#x\r\n", W25QXX_TYPE);
//		printf("w25q64 ok\r\n");
//	}
}
 
uint8_t SPI1_ReadWriteByte(u8 dat)
{
	u8 i, read_data;
	
	SPI_FLASH_SCLK_HIGH();	//ʱ��������
	
	for (i = 0; i< 8; i++)
	{
		read_data <<= 1;
		
		if (0x80 == ((dat << i)&0x80))
		{
			SPI_FLASH_MOSI_HIGH();
		}	
		else
		{
			 SPI_FLASH_MOSI_LOW();
		}
		SPI_FLASH_SCLK_LOW();	//����ʱ���ߣ��������������
		delay_us(1);
		SPI_FLASH_SCLK_HIGH();	//����ʱ���ߣ�׼����ȡ����
		delay_us(1);
		
		if (GPIO_ReadInputDataBit(FLASH_SPI_MISO_PORT, FLASH_SPI_MISO_PIN))
		{
			read_data |= 0x01;
		}
	}
	
	SPI_FLASH_SCLK_HIGH();	
	
	return read_data;
}
 
//��ȡW25QXX��״̬�Ĵ���
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
//TB,BP2,BP1,BP0:FLASH����д��������
//WEL:дʹ������
//BUSY:æ���λ(1,æ;0,����)
//Ĭ��:0x00
uint8_t W25QXX_ReadSR(void)
{
	u8 byte = 0;
	SPI_FLASH_CS_LOW();		/* ѡ��FLASH: CS�͵�ƽ */
	SPI1_ReadWriteByte(W25X_ReadStatusReg);
	byte = SPI1_ReadWriteByte(0xff);
	SPI_FLASH_CS_HIGH();
	
	return byte;
}
 
//дW25QXX״̬�Ĵ���
//ֻ��SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)����д!!!
void W25QXX_Write_SR(u8 sr)
{
	SPI_FLASH_CS_LOW();
	SPI1_ReadWriteByte(W25X_WriteStatusReg);
	SPI1_ReadWriteByte(sr);
	SPI_FLASH_CS_HIGH();
}
 
 
//W25QXXдʹ��
//��WEL��λ
void W25QXX_Write_Enable(void)
{
	SPI_FLASH_CS_LOW();
	SPI1_ReadWriteByte(W25X_WriteEnable);
	SPI_FLASH_CS_HIGH();
}
 
//W25QXXд��ֹ
//��WEL����
void W25QXX_Write_Disable(void)
{
	SPI_FLASH_CS_LOW();
	SPI1_ReadWriteByte(W25X_WriteDisable);
	SPI_FLASH_CS_HIGH();
}
 
//��ȡоƬID
//����ֵ����:
//0XEF13,��ʾоƬ�ͺ�ΪW25Q80
//0XEF14,��ʾоƬ�ͺ�ΪW25Q16
//0XEF15,��ʾоƬ�ͺ�ΪW25Q32
//0XEF16,��ʾоƬ�ͺ�ΪW25Q64
//0XEF17,��ʾоƬ�ͺ�ΪW25Q128
u16 W25QXX_ReadID(void)
{
	u16 temp;
	SPI_FLASH_CS_LOW();
	SPI1_ReadWriteByte(W25X_ManufactDeviceID);
	SPI1_ReadWriteByte(0x00);
	SPI1_ReadWriteByte(0x00);
	SPI1_ReadWriteByte(0x00);
	temp |= (SPI1_ReadWriteByte(0xff)<<8);
	temp |= SPI1_ReadWriteByte(0xff);
	SPI_FLASH_CS_HIGH();
	
	return temp;
}
 
//��ȡSPI FLASH
//��ָ����ַ��ʼ��ȡָ�����ȵ�����
//pBuffer:���ݴ洢��
//ReadAddr:��ʼ��ȡ�ĵ�ַ(24bit)
//NumByteToRead:Ҫ��ȡ���ֽ���(���65535)
void W25QXX_Read(u8 *pBuffer, u32 ReadAddr, u16 NumByteToRead)
{
	u16 i;
	SPI_FLASH_CS_LOW();
	SPI1_ReadWriteByte(W25X_ReadData);
	SPI1_ReadWriteByte((u8)(ReadAddr>>16));
	SPI1_ReadWriteByte((u8)(ReadAddr>>8));
	SPI1_ReadWriteByte((u8)ReadAddr);
	
	for (i=0; i<NumByteToRead; i++)
	{
		pBuffer[i] = SPI1_ReadWriteByte(0xff);
	}
	SPI_FLASH_CS_HIGH();
}
 
//SPI��һҳ(0~65535)��д������256���ֽڵ�����
//��ָ����ַ��ʼд�����256�ֽڵ�����
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���256),������Ӧ�ó�����ҳ��ʣ���ֽ���!!!
void W25QXX_Write_Page(u8 *pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
	u16 i;
	
	if (NumByteToWrite > 256)	/* ������� */
		NumByteToWrite = 256;
	
	W25QXX_Write_Enable();		//дʹ��
	SPI_FLASH_CS_LOW();
	SPI1_ReadWriteByte(W25X_PageProgram);
	SPI1_ReadWriteByte((u8)(WriteAddr>>16));
	SPI1_ReadWriteByte((u8)(WriteAddr>>8));
	SPI1_ReadWriteByte((u8)WriteAddr);
	
	for (i=0; i<NumByteToWrite; i++)
	{
		SPI1_ReadWriteByte(pBuffer[i]);
	}
	SPI_FLASH_CS_HIGH();
	W25QXX_Wait_Busy();	//�ȴ�д�����
	
}
 
//�޼���дSPI FLASH
//����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!
//�����Զ���ҳ����
//��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)
//CHECK OK
void W25QXX_Write_NoCheck(u8 *pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
	u16 pageremain;
	pageremain = 256 - WriteAddr%256;
	
	if (NumByteToWrite <= pageremain)
	{
		pageremain = NumByteToWrite;
	}
	
	while (1)
	{
		W25QXX_Write_Page(pBuffer, WriteAddr, pageremain);
		
		if (pageremain == NumByteToWrite)
		{
			break;
		}
		else
		{
			pBuffer += pageremain;
			WriteAddr += pageremain;
			
			NumByteToWrite -= pageremain;
			
			if (NumByteToWrite > 256)
			{
				pageremain = 256;
			}
			else
			{
				pageremain = NumByteToWrite;
			}
		}
	}
}
 
//дSPI FLASH
//��ָ����ַ��ʼд��ָ�����ȵ�����
//�ú�������������!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)
void W25QXX_Write(u8 *pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
	u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
	
	/* ȡ�࣬��writeAddr��SPI_FLASH_PageSize��������������AddrֵΪ0 */
	Addr = WriteAddr % SPI_FLASH_PageSize;
	count  = SPI_FLASH_PageSize - Addr;	//��count������ֵ���պÿ��Զ��뵽ҳ��ַ
	NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;	//�����Ҫд��������ҳ/
	NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;	//�����ʣ�಻��һҳ���ֽ���
	
	if (Addr == 0)	//WriteAddr�պð�ҳ����
	{
		if (NumOfPage == 0)	//����һҳ
		{
			W25QXX_Write_Page(pBuffer, WriteAddr, NumByteToWrite);
		}
		else	//���ݱ�һҳ��
		{
			while (NumOfPage--)	//�Ȱ�����ҳд��
			{
				W25QXX_Write_Page(pBuffer, WriteAddr, SPI_FLASH_PageSize);
				WriteAddr += SPI_FLASH_PageSize;
				pBuffer += SPI_FLASH_PageSize;
			}
			/* ���ж���Ĳ���һҳ�����ݣ�����д�� */
			W25QXX_Write_Page(pBuffer, WriteAddr, NumOfSingle);
		}
	}
	else	//WriteAddrû�а�ҳ����
	{
		if (NumOfPage == 0)	//����һҳ
		{
			/* ��ǰҳ��д�������������Խ�����д�� */
			if (NumOfSingle > count)
			{
				temp = NumOfSingle - count;
				//�Ƚ���ǰҳд��
				W25QXX_Write_Page(pBuffer, WriteAddr, count);
				
				WriteAddr += count;
				pBuffer += count;
				//�ٽ�ʣ�������д��
				W25QXX_Write_Page(pBuffer, WriteAddr, temp);
			}
			else	/* ��ǰҳ��д�����������Խ�����д�� */
			{
				W25QXX_Write_Page(pBuffer, WriteAddr, NumByteToWrite);
			}
		}
		else	//Ҫд������������һҳ
		{
			/*��ַ����������count�ֿ������������������*/
			NumByteToWrite -= count;
			NumOfPage = NumByteToWrite / SPI_FLASH_PageSize;
			NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;
			
			/* ��д��count�����ݣ�Ϊ��������һ��Ҫд�ĵ�ַ���� */
			W25QXX_Write_Page(pBuffer, WriteAddr, count);
			WriteAddr += count;
			pBuffer += count;
			
			while (NumOfPage--)
			{
				W25QXX_Write_Page(pBuffer, WriteAddr, SPI_FLASH_PageSize);
				WriteAddr += SPI_FLASH_PageSize;
				pBuffer += SPI_FLASH_PageSize;
			}
			
			if (NumOfSingle != 0)
			{
				W25QXX_Write_Page(pBuffer, WriteAddr, NumOfSingle);
			}
		}
	}
}	
 
//��������оƬ
//�ȴ�ʱ�䳬��...
void W25QXX_Erase_Chip(void)
{
	/* ����FLASHдʹ������ */
	W25QXX_Write_Enable();
	W25QXX_Wait_Busy();
 
	/* ���� Erase */
	SPI_FLASH_CS_LOW();		//ѡ��FLASH: CS�͵�ƽ
	SPI1_ReadWriteByte(W25X_ChipErase);	// �����������ָ��
	SPI_FLASH_CS_HIGH();	//ֹͣ�ź� FLASH: CS �ߵ�ƽ
 
	/* �ȴ��������*/
	W25QXX_Wait_Busy();
}
 
//����һ������
//Dst_Addr:������ַ ����ʵ����������
//����һ������������ʱ��:150ms
void W25QXX_Erase_Sector(u32 Dst_Addr)
{
	W25QXX_Write_Enable();
	W25QXX_Wait_Busy();
	
	SPI_FLASH_CS_LOW();
	SPI1_ReadWriteByte(W25X_SectorErase);	
	SPI1_ReadWriteByte((u8)(Dst_Addr >> 16));
	SPI1_ReadWriteByte((u8)(Dst_Addr >> 8));
	SPI1_ReadWriteByte((u8)Dst_Addr);
	SPI_FLASH_CS_HIGH();
	W25QXX_Wait_Busy();
}
 
//�ȴ�����
void W25QXX_Wait_Busy(void)
{
	while ((W25QXX_ReadSR() & 0x01) == 0x01);	// �ȴ�BUSYλ���
}
 
//�������ģʽ
void W25QXX_PowerDown(void)
{
	SPI_FLASH_CS_LOW();
	SPI1_ReadWriteByte(W25X_PowerDown);	
	SPI_FLASH_CS_HIGH();
	delay_us(3);
}
 
//����
void W25QXX_WAKEUP(void)
{
	SPI_FLASH_CS_LOW();
	SPI1_ReadWriteByte(W25X_ReleasePowerDown);	
	SPI_FLASH_CS_HIGH();
	delay_us(3);
}
