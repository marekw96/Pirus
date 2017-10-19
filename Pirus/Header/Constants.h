#pragma once
#include <string>

namespace Pirus
{
	using text = std::string;

	enum class CHILD_TYPE{NOT_ALLOWED,NONE,TAG,TEXT};

	enum class ALLOW_CHILDREN{YES,NO};
}