#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ftp_client.h"
#include "tools.h"

int main(int argc,const char* argv[])
{
	//	创建FTP对象
	FTPClient* ftp = create_FTPClient();

	//	连接服务器
	if(2 == argc)
	{
		if(connect_FTPClient(ftp,argv[1],21))
			return EXIT_FAILURE;
	}
	else if(3 == argc)
	{
		if(connect_FTPClient(ftp,argv[1],atoi(argv[2])))
			return EXIT_FAILURE;	
	}
	else
	{
		printf("User:./ftp xxx.xxx.xxx.xxx [port]\n");
		return EXIT_SUCCESS;
	}
	
	//	输入用户名
	char user[20] = {};
	printf("Name:");
	get_str(user,20);
	user_FTPClient(ftp,user);

	//	输入密码
	char pass[20] = {};
	printf("Password:");
	get_pass(pass,20,true);
	pass_FTPClient(ftp,pass);

	char cmd[10+PATH_MAX] = {};
	short cmd_size = sizeof(cmd);

	for(;;)
	{
		printf("ftp>");
		get_str(cmd,cmd_size); 

		if(0 == strcmp(cmd,"pwd"))
			pwd_FTPClient(ftp);
		else if(0 == strncmp(cmd,"mkdir ",6))
			mkdir_FTPClient(ftp,cmd+6);
		else if(0 == strncmp(cmd,"rmdir ",6))
			rmdir_FTPClient(ftp,cmd+6);
		else if(0 == strncmp(cmd,"cd ",3))
			cd_FTPClient(ftp,cmd+3);
		else if(0 == strcmp(cmd,"bye"))
			bye_FTPClient(ftp);
		else if(0 == strcmp(cmd,"ls"))
			ls_FTPClient(ftp);
		else if(0 == strncmp(cmd,"put ",4))
			put_FTPClient(ftp,cmd+4);
		else if(0 == strncmp(cmd,"get ",4))
			get_FTPClient(ftp,cmd+4);
		else if('!' == cmd[0])
			system(cmd+1);
		else
			printf("指令未定义!\n");
	
	}
}




