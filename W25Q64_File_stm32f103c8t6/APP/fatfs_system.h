#ifndef __FATFS_SYSTEM_H
#define __FATFS_SYSTEM_H

#include "stm32f10x.h"                  // Device header
#include "ff.h"

void FatFs_Check(void);				                    //�ж�FatFs�Ƿ���سɹ�����û�д���FatFs���ʽ��SD��

FRESULT f_mkdir_wrapper(const char *path);              //�����ļ���
FRESULT f_unlink_wrapper(const char *path);             //ɾ���ļ�
void write_file(const char *path, const char *content); //������д�ļ�
void read_file(const char *path);                       //���ļ�
void list_dir(const char *path);                        //�г�Ŀ¼�б�

void process_command(const char *cmd);

FRESULT traverse_directory(const char *dir_path);       //����Ŀ¼��ȫ���ļ�
#endif
