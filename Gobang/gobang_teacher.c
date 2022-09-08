#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getch.h>
#include <network.h>

//	棋盘数组
char board[15][15];
//	棋子坐标
char key_x = 7,key_y = 7;
//	棋子角色 黑'@' 白'$'
char role = '@';
//	初始化棋盘
void init_board(void)
{
	for(int i=0; i<15; i++)
	{
		for(int j=0; j<15;j++)
		{
			board[i][j] = '*';	
		}
	}
}

//	显示棋盘
void show_board(void)
{
	system("clear");
	for(int i=0; i<15; i++)
	{
		for(int j=0; j<15;j++)
		{
			printf(" %c",board[i][j]);
		}
		printf("\n");
	}
}

//	落子
void get_key(void)
{
	//	输入坐标落子操作
/*	for(;;)
	{
		printf("请%c输入棋子坐标:",role);	
		scanf("%hhd%hhd",&key_x,&key_y);
		//	检查坐标是否合法
		if(0 > key_x || 14 < key_x || 0 > key_y || 14 < key_y)
		{
			printf("坐标不合法，请重新输入\n");
			continue;
		}
		//	检查是否有棋子
		if('*' != board[key_x][key_y])
		{
			printf("该位置已有棋子，请重新输入\n");
			continue;
		}
		board[key_x][key_y] = role;
		return;
	}*/

	//	光标落子操作
	printf("请%c落子",role);
	for(;;)
	{
		printf("\33[%hhd;%hhdH",key_x+1,(key_y+1)*2);
		switch(getch())
		{
			case 183:	key_x>0  && key_x--;	break;
			case 184:	key_x<14 && key_x++;	break;
			case 186:	key_y>0 &&  key_y--;	break;
			case 185:	key_y<14 && key_y++;	break;
			case 10:
			{
				if('*' == board[key_x][key_y])
				{
					board[key_x][key_y] = role;
					return;
				}
				break;
			}
		}
	}
}

// 五子连珠，判断一个小方向
int func(int i,int j)
{
	int cnt = 0;
	for(int x=key_x+i,y=key_y+j;x>=0 && x<15 && y>=0 && y<15;x+=i,y+=j)
	{
		if(board[x][y] == role)
		{
			cnt++;	
		}
		else
		{
			return cnt;	
		}
	}
	return cnt;
}

//	检查是否五子连珠
int is_win(void)
{
	if(4 <= func(-1,-1) + func(1,1)) return 1;
	if(4 <= func(-1,0) + func(1,0)) return 1;
	if(4 <= func(0,-1) + func(0,1)) return 1;
	if(4 <= func(-1,1) + func(1,-1)) return 1;
	return 0;
}
int buf[3] = {};
int main(int argc,const char* argv[])
{
	NetWork* snw = init_nw(SOCK_STREAM, atoi(argv[1]), argv[2], true);
	if(snw == NULL)
	{
		perror("init_nw");
		return EXIT_FAILURE;
	}
	NetWork* cnw = NULL;
	
	cnw = accept_nw(snw);
	if(cnw == NULL)
	{
		perror("accept_nw");
		return EXIT_FAILURE;
	}
	init_board();
	show_board();
	for(;;)
	{
		printf("请%c落子\n", role);
		recv_nw(cnw, buf, sizeof(buf));
		board[buf[1]][buf[2]] = '@';
		show_board();
		if(buf[0])
		{
			printf("%c胜利\n", role);
			close_nw(snw);
			close_nw(cnw);
			return 0;
		}
		role = '$';
		get_key();
		buf[1] = key_x, buf[2] = key_y;
		if(is_win())
		{
			show_board();
			printf("%c胜利\n",role);
			buf[0] = 1;
			send_nw(cnw, buf, sizeof(buf)); 
			close_nw(snw);
			close_nw(cnw);
			return 0;
		}
		send_nw(cnw, buf, sizeof(buf)); 
		//	交换角色
		role = '@';
		show_board();
	}
}
