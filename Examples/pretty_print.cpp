#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include "Parser.h"

int main()
{
	std::ifstream t("ugly_html.htm");

	if (!t.is_open())
	{
		std::cout << "File not found. Exiting..." << std::endl;
		return 1;
	}

	std::string str((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());

	std::cout << "Before parsing: " << std:: endl;
	std::cout << str << std::endl;
	std::cout << std::endl;
	std::cout << "After parsing: " << std::endl;

	auto output = Pirus::parse(str);
	
	for (const auto& tag_parent : output)
	{
		std::cout << tag_parent << std::endl;
	}

	return 0;
}