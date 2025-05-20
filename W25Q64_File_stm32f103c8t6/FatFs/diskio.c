/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */
#include "spi_flash.h"
/* Definitions of physical drive number for each drive */
#define SPI_FLASH		0	//外部SPI
//#define DEV_RAM		0	/* Example: Map Ramdisk to physical drive 0 */
//#define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
//#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* 用于标识驱动器的物理驱动器号 */
)
{
	DSTATUS status = STA_NOINIT;
	
	switch (pdrv) {
		case SPI_FLASH :
			if (0XEF16 == W25QXX_ReadID())
			{
				status &= ~STA_NOINIT;
			}
			else
			{
				status = STA_NOINIT;
			}
			break;
			
		default:
			status = STA_NOINIT;
	}
	return status;
}

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	uint16_t i;
	DSTATUS status = STA_NOINIT;
 
	switch (pdrv) {
		
		case SPI_FLASH :
			W25QXX_Init();
			i = 500;
			while(--i);    //进行短暂的延时
			status = disk_status(SPI_FLASH);    
			break;
		
		default:
			status = STA_NOINIT;//这里返回的失败，说明上面没执行？
	}
	return status;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
 
	DRESULT status = RES_PARERR;
	
	switch (pdrv) {
		case SPI_FLASH :
			/* 左移12位也就是乘以4096，因为一个扇区4096个字节 */
			W25QXX_Read(buff, sector<<12, count<<12);
			status = RES_OK;
			break;
		
		default:
			status = RES_PARERR;
	}
 
	return status;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0
 
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	uint32_t write_addr;
	DRESULT status = RES_PARERR;
	
	if (!count){
		return RES_PARERR;		/* Check parameter */
	}
 
	switch (pdrv) {
		case SPI_FLASH :
			write_addr = sector << 12;
			W25QXX_Erase_Sector(write_addr);
			W25QXX_Write((u8 *)buff, write_addr, count<<12);
			status = RES_OK;
			break;
		
		default:
			status = RES_PARERR;
	}
 
	return status;
}
 
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
 
	DRESULT status = RES_PARERR;
	
	switch (pdrv) {
		case SPI_FLASH:
			switch(cmd)
			{
				/* 扇区数量 2048*4096/1024/1024=8MB */
				case GET_SECTOR_COUNT:
					*(DWORD *)buff = 2048;
					break;
				
				/* 扇区大小 */ 
				case GET_SECTOR_SIZE:
					*(WORD *)buff = 4096;
					break;
				
				/* 同时擦除扇区个数 */
				case GET_BLOCK_SIZE:
					*(DWORD *)buff = 1;
					break;
			}
			status = RES_OK;
			break;
			
		default:
			status = RES_PARERR;
	}
 
	return status;
}

