#ifndef __FATFS_SYSTEM_H
#define __FATFS_SYSTEM_H

#include "stm32f10x.h"                  // Device header
#include "ff.h"

void FatFs_Check(void);				                    //判断FatFs是否挂载成功，若没有创建FatFs则格式化SD卡

FRESULT f_mkdir_wrapper(const char *path);              //创建文件夹
FRESULT f_unlink_wrapper(const char *path);             //删除文件
void write_file(const char *path, const char *content); //创建和写文件
void read_file(const char *path);                       //读文件
void list_dir(const char *path);                        //列出目录列表

void process_command(const char *cmd);

FRESULT traverse_directory(const char *dir_path);       //遍历目录的全部文件
#endif
