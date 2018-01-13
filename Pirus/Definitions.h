#pragma once

#include <string>
#include <unordered_map>

namespace Pirus
{
	using text = std::wstring;
	using attributes_map = std::unordered_map<Pirus::text, Pirus::text>;

	enum class ALLOW_CHILDREN {YES, NO};
}