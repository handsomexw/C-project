#ifndef GUEST_H
#define GUEST_H

#include <iostream>
using namespace std;

class Guest
{
	string name;	//姓名
	char sex;		//性别
	string id;		//身份证号
public:
	Guest(const string& name,char sex,const string& id):name(name),sex(sex),id(id) {}
	
	void show(void)
	{
		cout << name << " " << sex << " " << id << endl;
	}
};

#endif//GUEST_H
