#include "hotel.h"

int main(int argc,const char* argv[])
{
	if(2 != argc)
	{
		cout << "User: ./HMS <path>" << endl;
		return 0;
	}
	Hotel& hotel = Hotel::getHotel();
	hotel.load(argv[1]);
	hotel.start();
}
