#ifndef ROOM_H
#define ROOM_H

#include "guest.h"

class Room
{
	short id;		//房号
	char type;		//房型(人数)
	double price;	//房价
	int guest_cnt;	//房客人数
	Guest** guests;	//房客指针数组

public:
	Room(short id,char type,double price):id(id),type(type),price(price),guest_cnt(0)
	{
		guests = new Guest*[type];	
	}
	
	//	比对房间号
	bool cmp_room_id(int room_id)
	{
		return id == room_id;
	}
	
	//	是否空房
	bool is_empty(void)
	{
		return 0 == guest_cnt;
	}
	
	//	入住
	bool check_in(Guest* guest)
	{
		if(type <= guest_cnt)	return false;
		guests[guest_cnt++] = guest;
		return true;
	}
	
	//	退房
	void check_out(void)
	{
		if(0 == guest_cnt)
		{
			cout << "房间为空" << endl;
			return;
		}
		
		for(int i=0; i<guest_cnt; i++)
		{
			delete guests[i];	
		}
		guest_cnt = 0;
		cout << "退房成功" << endl;
	}
	
	void show(void)
	{
		cout << id << " " << type+0 << " " << price << endl;
		for(int i=0; i<guest_cnt; i++)
		{
			cout << "房客" << i+1 << ": ";
			guests[i]->show();
		}	
	}
	
};

#endif//ROOM_H
