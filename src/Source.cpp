#include "../include/list.h"

#include <iostream>

int main()
{
	blk::list<int> list;
	list.push_back(2);
	list.push_front(1);
	for (auto i : list)
	{
		std::cout << i << ' ';
	}
	std::cout << std::endl;
	return 0;
}