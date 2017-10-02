#include <iostream>
#include "Tag.h"

int main()
{
	Pirus::Tag test_node("img", 0);
	std::cout << test_node.get_name();

	return 0;
}