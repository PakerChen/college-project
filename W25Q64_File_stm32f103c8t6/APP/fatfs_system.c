#include "stm32f10x.h"                  // Device header


#include "ff.h"

#include <stdio.h>
#include <string.h>

FATFS 		sFLASH_FatFs; 											//�ļ�ϵͳ����
FIL file;
BYTE work[FF_MAX_SS]; 

//����FatFs�ļ�ϵͳ�Ĺ���
void FatFs_Check(void)												//�ж�FatFs�Ƿ���سɹ�����û�д���FatFs���ʽ��SD��
{
	FRESULT MyFile_Res;       										//������� 
	
	MyFile_Res = f_mount(&sFLASH_FatFs,"0:",0);						//��ʼ��SPI Flash��������FatFs
	
	if (MyFile_Res == FR_OK)										//�ж��Ƿ���سɹ�
	{
		printf("\r\nSPI Flash�ļ�ϵͳ���سɹ�\r\n");
	}
	else		
	{
		printf("SPI Flash��δ�����ļ�ϵͳ��������ʽ��\r\n");
		
		MyFile_Res = f_mkfs("0:",0,work,sizeof(work));				//��ʽ����FAT32����Ĭ�ϴ�С16K
		
		if (MyFile_Res == FR_OK)									//�ж��Ƿ��ʽ���ɹ�
		{
			printf("SPI Flash��ʽ���ɹ���\r\n");
		}
		else
		{
			printf("��ʽ��ʧ�ܣ���������SPI Flash��\r\n");
		}
	}
}

FRESULT f_mkdir_wrapper(const char *path) 
{
    FRESULT res = f_mkdir(path);									//�����ļ���
	if(res != FR_OK)
	{
		printf("���ڵĲ����Ǵ����ļ��У�����ʧ����\r\n");
	}
	else
	{
		printf("�ļ��д����ɹ�\r\n");
	}
    return res;
}

FRESULT f_unlink_wrapper(const char *path) 
{
    FRESULT res = f_unlink(path);									//ɾ���ļ�
	if(res != FR_OK)
	{
    	printf("���ڵĲ�����ɾ���ļ�������ʧ����\r\n");
	}
	else
	{
		printf("�ļ�ɾ���ɹ�\r\n");
	}
    return res;
}

// �ļ�д�뺯��
void write_file(const char *path, const char *content) 
{
    FRESULT res = f_open(&file, path, FA_WRITE | FA_OPEN_APPEND);	//д�ļ����ļ��������򴴽�
    if (res == FR_OK) 
	{
        UINT bw;													//Ҫд����ֽ���
        f_write(&file, content, strlen(content), &bw);				//д�ļ�
        f_close(&file);												//�ر��ļ�
		if(bw != strlen(content))
        {
			printf("���ڵĲ�����д�ļ��������ֳ�������\r\n");
		}
		else
		{
			printf("�ļ�д��ɹ�\r\n");
		}
    } 
	else 
	{
		printf("���ڵĲ�����д�ļ�������ʧ����\r\n");
    }
}

// �ļ���ȡ����
void read_file(const char *path) 
{
    char buffer[128];
	
    FRESULT res = f_open(&file, path, FA_READ);						//���ļ���
    if (res == FR_OK) 
	{
        UINT br;
        f_read(&file, buffer, sizeof(buffer), &br);
        buffer[br] = '\0';
        printf("%s\r\n",buffer);									//���ļ����ݴ�ӡ����
        f_close(&file);
    } 
	else 
	{
		printf("���ڵĲ����Ƕ��ļ�������ʧ����\r\n");
    }
}

// �г�Ŀ¼
void list_dir(const char *path) 
{
    DIR dir;
    FILINFO fno;

    FRESULT res = f_opendir(&dir, path);							//��һ��Ŀ¼
    if (res == FR_OK) 
	{
        char list[256] = "";
        while (f_readdir(&dir, &fno) == FR_OK && fno.fname[0]) 
		{
            strcat(list, fno.fname);								//�����������ʵ���ַ�����β��ƴ��
            strcat(list, "\r\n");
        }
        printf("%s",list);
        f_closedir(&dir);
    } 
	else 
	{
        printf("���ڵĲ������г�Ŀ¼������ʧ����\r\n");
    }
}

/*
mkdir /test          				# ����Ŀ¼
write /test/1.txt Hello FATFS  		# д������---ʵ����׷��
read /test/1.txt     				# ��ȡ����
ls /test             				# �г�Ŀ¼
rm /test/1.txt       				# ɾ���ļ�
rmdir /test          				# ɾ����Ŀ¼
*/

void process_command(const char *cmd) 
{
    char operation[16], path[64], content[128];										//�ҹ涨����ĸ�ʽ�� ����/·��/����

	//sscanf����---���������Ǵ��ַ������ȡ��ʽ��������
	//ʹ���������������ȡ��[����/·��/����]
	//�ָ������ǿհ��ַ�
	//����ֵ�����շ��سɹ���ֵ���ֶ�����
    int args = sscanf(cmd, "%15s %63s %127[^\n]", operation, path, content);		//�ָ�� ����/·��/����

    // �Զ���ȫ�������ţ��罫"/test"��ȫΪ"0:/test"��
    char full_path[64];
    if (path[0] != '1')                                                             //����ַ�������λ����0����Ҫ����
	{
		snprintf(full_path, sizeof(full_path), "%s%s", "0:", path);					//��ȫ1:
	}
    else 
	{
		strcpy(full_path, path);
	}

    // ���ݲ������͵���API
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
