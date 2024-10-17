#include <iostream>
#include <sstream>

#include "commandhandler.h"
#include "asyncsoc.h"

int main(int argc, char *argv[])
{
	auto ret_1 = AsyncSoc::connect(3);
	auto ret_2 = AsyncSoc::connect(4);
	AsyncSoc::receive(ret_1, "1", 1);
	AsyncSoc::receive(ret_2, "2", 1);
	AsyncSoc::receive(ret_1, "3", 1);
	AsyncSoc::receive(ret_2, "4", 1);
	AsyncSoc::receive(ret_1, "5", 1);
	AsyncSoc::receive(ret_2, "6", 1);
	AsyncSoc::receive(ret_1, "7", 1);
	AsyncSoc::receive(ret_2, "8", 1);
	AsyncSoc::receive(ret_1, "9", 1);
	AsyncSoc::receive(ret_2, "10", 2);
	AsyncSoc::receive(ret_1, "11", 2);
	AsyncSoc::receive(ret_2, "12", 2);
	AsyncSoc::receive(ret_1, "13", 2);
	AsyncSoc::receive(ret_2, "14", 2);
	AsyncSoc::receive(ret_1, "15", 2);
	AsyncSoc::receive(ret_2, "16", 2);
	AsyncSoc::receive(ret_1, "17", 2);
	AsyncSoc::receive(ret_2, "18", 2);
	AsyncSoc::receive(ret_2, "19", 2);
	AsyncSoc::receive(ret_1, "20", 2);
	AsyncSoc::receive(ret_2, "21", 2);
	AsyncSoc::receive(ret_1, "22", 2);

	AsyncSoc::disconnect(ret_1);
	AsyncSoc::disconnect(ret_2);

	return 0;
}
