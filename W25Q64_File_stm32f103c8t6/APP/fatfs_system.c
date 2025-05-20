#include "stm32f10x.h"                  // Device header


#include "ff.h"

#include <stdio.h>
#include <string.h>

FATFS 		sFLASH_FatFs; 											//文件系统对象
FIL file;
BYTE work[FF_MAX_SS]; 

//进行FatFs文件系统的挂载
void FatFs_Check(void)												//判断FatFs是否挂载成功，若没有创建FatFs则格式化SD卡
{
	FRESULT MyFile_Res;       										//操作结果 
	
	MyFile_Res = f_mount(&sFLASH_FatFs,"0:",0);						//初始化SPI Flash，并挂载FatFs
	
	if (MyFile_Res == FR_OK)										//判断是否挂载成功
	{
		printf("\r\nSPI Flash文件系统挂载成功\r\n");
	}
	else		
	{
		printf("SPI Flash还未创建文件系统，即将格式化\r\n");
		
		MyFile_Res = f_mkfs("0:",0,work,sizeof(work));				//格式化，FAT32，簇默认大小16K
		
		if (MyFile_Res == FR_OK)									//判断是否格式化成功
		{
			printf("SPI Flash格式化成功！\r\n");
		}
		else
		{
			printf("格式化失败，请检查或更换SPI Flash！\r\n");
		}
	}
}

FRESULT f_mkdir_wrapper(const char *path) 
{
    FRESULT res = f_mkdir(path);									//创建文件夹
	if(res != FR_OK)
	{
		printf("现在的操作是创建文件夹，但是失败了\r\n");
	}
	else
	{
		printf("文件夹创建成功\r\n");
	}
    return res;
}

FRESULT f_unlink_wrapper(const char *path) 
{
    FRESULT res = f_unlink(path);									//删除文件
	if(res != FR_OK)
	{
    	printf("现在的操作是删除文件，但是失败了\r\n");
	}
	else
	{
		printf("文件删除成功\r\n");
	}
    return res;
}

// 文件写入函数
void write_file(const char *path, const char *content) 
{
    FRESULT res = f_open(&file, path, FA_WRITE | FA_OPEN_APPEND);	//写文件，文件不存在则创建
    if (res == FR_OK) 
	{
        UINT bw;													//要写入的字节数
        f_write(&file, content, strlen(content), &bw);				//写文件
        f_close(&file);												//关闭文件
		if(bw != strlen(content))
        {
			printf("现在的操作是写文件，但发现出现问题\r\n");
		}
		else
		{
			printf("文件写入成功\r\n");
		}
    } 
	else 
	{
		printf("现在的操作是写文件，但是失败了\r\n");
    }
}

// 文件读取函数
void read_file(const char *path) 
{
    char buffer[128];
	
    FRESULT res = f_open(&file, path, FA_READ);						//打开文件读
    if (res == FR_OK) 
	{
        UINT br;
        f_read(&file, buffer, sizeof(buffer), &br);
        buffer[br] = '\0';
        printf("%s\r\n",buffer);									//将文件内容打印出来
        f_close(&file);
    } 
	else 
	{
		printf("现在的操作是读文件，但是失败了\r\n");
    }
}

// 列出目录
void list_dir(const char *path) 
{
    DIR dir;
    FILINFO fno;

    FRESULT res = f_opendir(&dir, path);							//打开一个目录
    if (res == FR_OK) 
	{
        char list[256] = "";
        while (f_readdir(&dir, &fno) == FR_OK && fno.fname[0]) 
		{
            strcat(list, fno.fname);								//这个函数可以实现字符串的尾部拼接
            strcat(list, "\r\n");
        }
        printf("%s",list);
        f_closedir(&dir);
    } 
	else 
	{
        printf("现在的操作是列出目录，但是失败了\r\n");
    }
}

/*
mkdir /test          				# 创建目录
write /test/1.txt Hello FATFS  		# 写入内容---实际是追加
read /test/1.txt     				# 读取内容
ls /test             				# 列出目录
rm /test/1.txt       				# 删除文件
rmdir /test          				# 删除空目录
*/

void process_command(const char *cmd) 
{
    char operation[16], path[64], content[128];										//我规定命令的格式是 操作/路径/内容

	//sscanf函数---它的作用是从字符串里读取格式化的数据
	//使用这个函数可以提取出[操作/路径/内容]
	//分割依据是空白字符
	//返回值是最终返回成功赋值的字段总数
    int args = sscanf(cmd, "%15s %63s %127[^\n]", operation, path, content);		//分割出 操作/路径/内容

    // 自动补全驱动器号（如将"/test"补全为"0:/test"）
    char full_path[64];
    if (path[0] != '1')                                                             //如果字符串的首位不是0则需要补上
	{
		snprintf(full_path, sizeof(full_path), "%s%s", "0:", path);					//补全1:
	}
    else 
	{
		strcpy(full_path, path);
	}

    // 根据操作类型调用API
    if (strcmp(operation, "mkdir") == 0 && args >= 2) 
	{
        f_mkdir_wrapper(full_path);
    } 
	else if (strcmp(operation, "rmdir") == 0 && args >= 2) 
	{
        f_unlink_wrapper(full_path);
    } 
	else if (strcmp(operation, "rm") == 0 && args >= 2) 
	{
        f_unlink_wrapper(full_path);
    } 
	else if (strcmp(operation, "write") == 0 && args >= 3) 
	{
        write_file(full_path, content);
    }
	else if (strcmp(operation, "read") == 0 && args >= 2) 
	{
        read_file(full_path);
    } 
	else if (strcmp(operation, "ls") == 0 && args >= 1) 
	{
        list_dir(args >= 2 ? full_path : "0:");
    } 
	else 
	{
        printf("ERROR: Invalid command\r\n");
    }
}
