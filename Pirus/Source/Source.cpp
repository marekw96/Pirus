#include <iostream>
#include "../Header/Node.h"

using namespace std;

int main()
{
	Pirus::Node test_node("img", 0);
	cout << test_node.get_name();

	return 0;
}