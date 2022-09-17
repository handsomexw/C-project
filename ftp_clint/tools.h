#ifndef TOOLS_H
#define TOOLS_H

#include <stdio.h>
#include <stdbool.h>

#define ERROR(func) printf("file:%s func:%s line:%d %m\n",__FILE__,func,__LINE__)

//	从键盘获取字符串
char* get_str(char* str,size_t len);
//	从键盘输入密码
char* get_pass(char* pass,size_t len,bool is_show);
//	文件数据发送
void file_oi(int ofd,int ifd);
#endif//TOOLS_H
