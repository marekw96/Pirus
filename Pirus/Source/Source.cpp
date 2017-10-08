#include <iostream>
#include "Tag.h"
#include "Splitter.h"

int main()
{
	auto str = "<a href=\"index.html\">TEXT</a>";
	Pirus::Splitter s;
	s(str);

	auto& fragments = s.get_fragments();
	for (auto& f : fragments)
	{
		std::cout << "type: " << static_cast<int>(f.type) << std::endl;
		std::cout << "level: " << f.level << std::endl;
		std::cout << "value: " << f.value << std::endl<< std::endl;
	}
	return 0;
}