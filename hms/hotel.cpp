#include <cstdio>
#include <cstdlib>
#include "tools.h"
#include "hotel.h"
#include "guest.h"

Hotel Hotel::hotel;	//类外定义静态成员变量

//	加载房间信息
bool Hotel::load(const char* path)
{
	FILE* frp = fopen(path,"r");
	if(NULL == frp)
	{
		perror("fopen");
		return false;
	}
	
	fscanf(frp,"%hhd",&level_cnt);	//加载层数
	level_room = new char[level_cnt];	//分配内存
	rooms = new Room**[level_cnt];
	
	for(int i=0; i<level_cnt; i++)
	{
		fscanf(frp,"%hhd",&level_room[i]);
		rooms[i] = new Room*[level_room[i]];
		for(int j=0; j<level_room[i]; j++)
		{
			short id;
			char type;
			double price;
			fscanf(frp,"%hd %hhd %lf",&id,&type,&price);
			rooms[i][j] = new Room(id,type,price);
			//rooms[i][j]->show();
		}
	}
	return true;
}
	
//	启动程序
void Hotel::start(void)
{
	while(true)
	{
		switch(menu())
		{
			case '1':	check_in();	break;
			case '2': 	check_out();break;
			case '3': 	list_empty_room(); break;
			case '4':	query_room();	break;
			case '5':	return;
		}
	}
}

//	菜单
int Hotel::menu(void)
{
	system("clear");
	cout << "***欢迎使用指针酒店管理系统***" << endl;
	cout << "1、入住              2、退房" << endl;
	cout << "3、查看              4、查询" << endl;
	cout << "5、退出" << endl;
	cout << "**************************" << endl;
	return get_cmd('1','5');
}

//	入住
void Hotel::check_in(void)
{
	int room_id;
	cout << "请输入要入住的房间号：";
	cin >> room_id;
	Room* room = find_room(room_id);
	if(NULL == room)
	{
		cout << "未找到房间:" << room_id << " 请检查！" << endl;
		anykey_continue();
		return;	
	}
	
	string name,man_id;
	char sex;
	cout << "请输入房客姓名、性别、身份证号:";
	cin >> name >> sex >> man_id;
	Guest* guest = new Guest(name,sex,man_id);
	
	if(room->check_in(guest))
		cout << "入住成功" << endl;
	else
	{
		cout << "该房间已满" << endl;
		delete guest;
	}
	anykey_continue();
}

//	退房
void Hotel::check_out(void)
{
	int room_id;
	cout << "请输入要退房的房间号：";
	cin >> room_id;
	Room* room = find_room(room_id);
	if(NULL == room)
	{
		cout << "未找到房间:" << room_id << " 请检查！" << endl;
		anykey_continue();
		return;	
	}
	
	room->check_out();
	anykey_continue();
}

//	查看空房
void Hotel::list_empty_room(void)
{
	for(int i=0; i<level_cnt; i++)
	{
		for(int j=0; j<level_room[i]; j++)
		{
			if(rooms[i][j]->is_empty())
			{
				rooms[i][j]->show();
			}
		}
	}
	anykey_continue();
}

//	查询
void Hotel::query_room(void)
{
	for(int i=0; i<level_cnt; i++)
	{
		for(int j=0; j<level_room[i]; j++)
		{
			if(!(rooms[i][j]->is_empty()))
			{
				rooms[i][j]->show();
			}
		}
	}
	anykey_continue();
}

//	根据房号查找房间
Room* Hotel::find_room(int room_id)
{
	for(int i=0; i<level_cnt; i++)
	{
		for(int j=0; j<level_room[i]; j++)
		{
			if(rooms[i][j]->cmp_room_id(room_id))
			{
				return rooms[i][j];
			}
		}
	}
	return NULL;
}



