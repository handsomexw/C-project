#include <getch.h>
#include "tools.h"

//	从键盘获取字符串
char* get_str(char* str,size_t len)
{
	if(NULL == str || 0 == len) return NULL;

	size_t index = 0;

	while(index < len-1)
	{
		char key_val = getch();
		if(10 == key_val) break;
		if(127 == key_val)
		{
			if(0 < index)
			{
				index--;
				printf("\b \b");	//显示退格效果
			}	
			continue;
		}
		printf("%c",key_val);
		str[index++] = key_val;
	}
	str[index] = '\0';
	printf("\n");

	//	清理输入缓冲区
	stdin->_IO_read_ptr = stdin->_IO_read_end;

	return str;
}

//	从键盘输入密码
char* get_pass(char* pass,size_t len,bool is_show)
{
	if(NULL == pass || 0 == len) return NULL;

	size_t index = 0;

	while(index < len-1)
	{
		char key_val = getch();
		if(10 == key_val) break;
		if(127 == key_val)
		{
			if(0 < index)
			{
				index--;
				if(is_show) printf("\b \b");	//显示退格效果
			}	
			continue;
		}
		if(is_show) printf("*");
		pass[index++] = key_val;
	}
	pass[index] = '\0';
	printf("\n");
	//	清理输入缓冲区
	stdin->_IO_read_ptr = stdin->_IO_read_end;
	return pass;
}

//	文件数据发送
void file_oi(int ofd,int ifd)
{
	char buf[4096] = {};
	int ret = 0;
	while((ret = read(ofd,buf,sizeof(buf))))
	{
		write(ifd,buf,ret);	
	}
}






