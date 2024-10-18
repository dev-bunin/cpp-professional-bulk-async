#include <iostream>
#include <sstream>

#include "async.h"

void test()
{
	auto ret_1 = async::connect(3);
	auto ret_2 = async::connect(3);

	async::receive(ret_2, "cmd1", 4);
	async::receive(ret_1, "cmd1", 4);
	async::receive(ret_1, "cmd2", 4);
	async::receive(ret_2, "cmd2", 4);
	async::receive(ret_1, "{", 1);
	async::receive(ret_1, "cmd3", 4);
	async::receive(ret_2, "cmd3", 4);
	async::receive(ret_1, "cmd4", 4);
	async::receive(ret_1, "}", 1);
	async::receive(ret_2, "cmd4", 4);
	async::receive(ret_1, "{", 1);
	async::receive(ret_1, "cmd5", 4);
	async::receive(ret_1, "cmd6", 4);
	async::receive(ret_2, "cmd5", 4);
	async::receive(ret_1, "{", 1);
	async::receive(ret_1, "cmd7", 4);
	async::receive(ret_1, "cmd8", 4);
	async::receive(ret_1, "}", 1);
	async::receive(ret_1, "cmd9", 4);
	async::receive(ret_1, "}", 1);
	async::disconnect(ret_2);
	async::receive(ret_1, "{", 1);
	async::receive(ret_1, "cmd10", 5);
	async::receive(ret_1, "cmd11", 5);

	async::disconnect(ret_1);
}

int main()
{
	test();
	auto ret_1 = async::connect(3);
	auto ret_2 = async::connect(4);
	async::receive(ret_1, "1", 1);
	async::receive(ret_2, "2", 1);
	async::receive(ret_1, "3", 1);
	async::receive(ret_2, "4", 1);
	async::receive(ret_1, "5", 1);
	async::receive(ret_2, "6", 1);
	async::receive(ret_1, "7", 1);
	async::receive(ret_2, "8", 1);
	async::receive(ret_1, "9", 1);
	async::receive(ret_2, "10", 2);
	async::receive(ret_1, "11", 2);
	async::receive(ret_2, "12", 2);
	async::receive(ret_1, "13", 2);
	async::receive(ret_2, "14", 2);
	async::receive(ret_1, "15", 2);
	async::receive(ret_2, "16", 2);
	async::receive(ret_1, "17", 2);
	async::receive(ret_2, "18", 2);
	async::receive(ret_2, "19", 2);
	async::receive(ret_1, "20", 2);
	async::receive(ret_2, "21", 2);
	async::receive(ret_1, "22", 2);

	async::disconnect(ret_1);
	async::disconnect(ret_2);

	return 0;
}
