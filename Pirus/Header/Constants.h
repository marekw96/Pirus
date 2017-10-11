#pragma once
#include <string>

namespace Pirus
{
	using string = std::string;
	using text = std::string;

	using attribute = std::pair<string,string>;

	enum class CHILD_TYPE{NOT_ALLOWED,NONE,TAG,TEXT};
}