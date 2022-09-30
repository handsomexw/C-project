#ifndef HOTEL_H
#define HOTEL_H

#include "room.h"

class Hotel
{
	static Hotel hotel;	//单例模式 静态酒店类对象
	char level_cnt;		//层数
	char* level_room;	//每层房间数
	Room*** rooms;		//房间类指针
	
	Hotel(void) {}
	Hotel(const Hotel& that) {}	//构造函数私有化
	
public:
	static Hotel& getHotel(void)//静态成员函数获取静态成员变量
	{
		return hotel; 
	}
	
	//	加载房间信息
	bool load(const char* path);
	
	//	启动程序
	void start(void);
	//	菜单
	int menu(void);
	//	入住
	void check_in(void);
	//	退房
	void check_out(void);
	//	查看空房
	void list_empty_room(void);
	//	查询
	void query_room(void);
	//	根据房号查找房间
	Room* find_room(int room_id);
	
	 	
};

#endif//HOTEL_H
