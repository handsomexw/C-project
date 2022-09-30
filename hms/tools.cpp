#include <getch.h>
#include "tools.h"

int get_cmd(char start,char end)
{
	puts("请输入指令:");
	for(;;)
	{
		char cmd = getch();
		if(start <= cmd && end >= cmd)
		{
			printf("%c\n",cmd);
			return cmd;
		}
	}
}

void anykey_continue(void)
{
	stdin->_IO_read_ptr = stdin->_IO_read_end;
	cout << "请输入任意键继续..." << endl;
	getch();
}


