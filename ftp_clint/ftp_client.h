#ifndef FTP_CLIENT_H
#define FTP_CLIENT_H

#include <stdint.h>
#include <limits.h>

#define BUF_SIZE (4096)

typedef struct sockaddr* SP;

typedef struct FTPClient
{
	int cli_sock;	//	命令通道socket
	int cli_pasv;	//	数据通道socket
	char* buf;		//	命令、执行结果、数据
	char server_path[PATH_MAX];	//	服务器工作路径
	char client_path[PATH_MAX];	//	客户端工作路径
}FTPClient;

//	创建FTP客户端对象 分配内存
FTPClient* create_FTPClient(void);

//	销毁FTP客户端对象 释放内存
void destroy_FTPClient(FTPClient* ftp);

//	连接服务器
int connect_FTPClient(FTPClient* ftp,const char* ip,uint16_t port);

//	向服务器发送用户名
void user_FTPClient(FTPClient* ftp,const char* user);

//	向服务器发送密码
void pass_FTPClient(FTPClient* ftp,const char* pass);

//	pwd
void pwd_FTPClient(FTPClient* ftp);

//	mkdir
void mkdir_FTPClient(FTPClient* ftp,const char* dir);

//	rmdir
void rmdir_FTPClient(FTPClient* ftp,const char* dir);

//	cd
void cd_FTPClient(FTPClient* ftp,const char* path);

//	bye
void bye_FTPClient(FTPClient* ftp);

//	ls
void ls_FTPClient(FTPClient* ftp);

//	put
void put_FTPClient(FTPClient* ftp,const char* file);

//	get
void get_FTPClient(FTPClient* ftp,const char* file);

#endif//FTP_CLIENT_H
