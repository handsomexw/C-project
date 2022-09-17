#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "ftp_client.h"
#include "tools.h"

//	让socket连接服务器
static int _connect(int sockfd,const char* ip,uint16_t port)
{
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);

	if(connect(sockfd,(SP)&addr,sizeof(addr)))
	{
		ERROR("connect");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

//	接收服务器响应 并判断状态码
static int recv_status(FTPClient* ftp,int status,bool is_die)
{
	size_t ret_size = recv(ftp->cli_sock,ftp->buf,BUF_SIZE,0);
	if(0 >= ret_size)
	{
		ERROR("recv");
		destroy_FTPClient(ftp);
		exit(EXIT_FAILURE);
	}

	ftp->buf[ret_size] = '\0';
	printf("%s\n",ftp->buf);

	int ret_status = 0;
	sscanf(ftp->buf,"%d",&ret_status);
	if(status != ret_status && is_die)
	{
		destroy_FTPClient(ftp);
		exit(EXIT_FAILURE);
	}
	
	return status == ret_status ? EXIT_SUCCESS:EXIT_FAILURE;
}

//	客户端发送指令
static void send_cmd(FTPClient* ftp)
{
	size_t ret_size = send(ftp->cli_sock,ftp->buf,strlen(ftp->buf),0);
	if(0 >= ret_size)
	{
		ERROR("send");
		destroy_FTPClient(ftp);
		exit(EXIT_FAILURE);
	}
}

//	开启并连接数据通道 
static int _pasv(FTPClient* ftp)
{
	sprintf(ftp->buf,"PASV\n");
	send_cmd(ftp);
	if(recv_status(ftp,227,false)) return EXIT_FAILURE;

	uint8_t ip1,ip2,ip3,ip4,port1,port2;
	char ip[16] = {};
	uint16_t port;

	sscanf(strchr(ftp->buf,'(')+1,"%hhu,%hhu,%hhu,%hhu,%hhu,%hhu",&ip1,&ip2,&ip3,&ip4,&port1,&port2);
	sprintf(ip,"%hhu.%hhu.%hhu.%hhu",ip1,ip2,ip3,ip4);
	port = port1*256+port2;

	ftp->cli_pasv = socket(AF_INET,SOCK_STREAM,0);
	if(0 > ftp->cli_pasv)
	{
		ERROR("socket");	
		return EXIT_FAILURE;
	}
	
	return _connect(ftp->cli_pasv,ip,port);
}

//	创建FTP客户端对象 分配内存
FTPClient* create_FTPClient(void)
{
	FTPClient* ftp = malloc(sizeof(FTPClient));
	if(NULL == ftp) return NULL;

	ftp->cli_sock = socket(AF_INET,SOCK_STREAM,0);
	if(0 > ftp->cli_sock)
	{
		ERROR("socket");
		free(ftp);
		return NULL;
	}

	ftp->buf = malloc(BUF_SIZE);
	return ftp;	
}

//	销毁FTP客户端对象 释放内存
void destroy_FTPClient(FTPClient* ftp)
{
	close(ftp->cli_sock);
	close(ftp->cli_pasv);
	free(ftp->buf);
	free(ftp);
}

//	连接服务器
int connect_FTPClient(FTPClient* ftp,const char* ip,uint16_t port)
{
	if(_connect(ftp->cli_sock,ip,port))
	{
		return EXIT_FAILURE;	
	}

	recv_status(ftp,220,true);
	return EXIT_SUCCESS;
}

//	向服务器发送用户名
void user_FTPClient(FTPClient* ftp,const char* user)
{
	sprintf(ftp->buf,"USER %s\n",user);
	send_cmd(ftp);
	recv_status(ftp,331,true);
}

//	向服务器发送密码
void pass_FTPClient(FTPClient* ftp,const char* pass)
{
	sprintf(ftp->buf,"PASS %s\n",pass);
	send_cmd(ftp);
	recv_status(ftp,230,true);
}

//	pwd
void pwd_FTPClient(FTPClient* ftp)
{
	sprintf(ftp->buf,"PWD\n");
	send_cmd(ftp);
	recv_status(ftp,257,false);

	strcpy(ftp->server_path,strchr(ftp->buf,'\"')+1);
	*strchr(ftp->server_path,'\"') = '\0';
	printf("server_path:%s\n",ftp->server_path);
	
}

//	mkdir
void mkdir_FTPClient(FTPClient* ftp,const char* dir)
{
	sprintf(ftp->buf,"MKD %s\n",dir);
	send_cmd(ftp);
	recv_status(ftp,257,false);
}

//	rmdir
void rmdir_FTPClient(FTPClient* ftp,const char* dir)
{
	sprintf(ftp->buf,"RMD %s\n",dir);
	send_cmd(ftp);
	recv_status(ftp,250,false);
}

//	cd
void cd_FTPClient(FTPClient* ftp,const char* path)
{
	sprintf(ftp->buf,"CWD %s\n",path);
	send_cmd(ftp);
	recv_status(ftp,250,false);
}

//	bye
void bye_FTPClient(FTPClient* ftp)
{
	sprintf(ftp->buf,"QUIT\n");
	send_cmd(ftp);
	recv_status(ftp,221,false);
	destroy_FTPClient(ftp);
	exit(EXIT_SUCCESS);
}

//	ls
void ls_FTPClient(FTPClient* ftp)
{
	if(_pasv(ftp)) return;
	
	sprintf(ftp->buf,"LIST -al\n");
	send_cmd(ftp);
	if(recv_status(ftp,150,false))	
	{	
		close(ftp->cli_pasv);
		return;
	}

	//	接收数据
	file_oi(ftp->cli_pasv,STDOUT_FILENO);
	close(ftp->cli_pasv);
	recv_status(ftp,226,false);
}

//	put
void put_FTPClient(FTPClient* ftp,const char* file)
{
	int file_fd = open(file,O_RDONLY);
	if(0 > file_fd)
	{
		ERROR("open");
		return;
	}

	if(_pasv(ftp)) 
	{
		close(file_fd);
		return;
	}

	sprintf(ftp->buf,"STOR %s\n",file);
	send_cmd(ftp);
	if(recv_status(ftp,150,false))
	{
		close(file_fd);
		close(ftp->cli_pasv);
		return;
	}
//	正常的系统IO
//	file_oi(file_fd,ftp->cli_pasv);

	//	大文件传输 考虑使用零拷贝 减少文件拷贝次数 用户态和内核态的转换次数 从而达到大大节约传输时间和提高效率
	struct stat st = {};
	fstat(file_fd,&st);
	
	//	第一个参数必须是socket文件 进行发送
	//	第二个参数必须是一个打开的文件描述符 进行读取
	//	第三个参数指定文件读取位置，NULL 从头读
	//	发送的字节数
	sendfile(ftp->cli_pasv,file_fd,NULL,st.st_size);

	close(file_fd);
	close(ftp->cli_pasv);
	recv_status(ftp,226,false);
}

//	get
void get_FTPClient(FTPClient* ftp,const char* file)
{
	printf("%s\n",__func__);
	
}

