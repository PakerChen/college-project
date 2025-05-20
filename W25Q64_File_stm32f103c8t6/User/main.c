#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "spi_flash.h"
#include "Serial.h"

#include "fatfs_system.h"

int main(void)
{
	OLED_Init();
	Serial_Init();
	W25QXX_Init();
	FatFs_Check();
	
	OLED_ShowString(0,0,"FATFS Test",OLED_8X16);
	OLED_Update();
	
	while (1)
	{
		if(Serial_RxFlag == 1)
		{
			Serial_RxFlag = 0;
			process_command(Serial_RxPacket);
		}
	}
}
