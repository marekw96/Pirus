#pragma once

#include <stdexcept>

namespace Pirus
{
	class ChildrenNotAllowed : public std::runtime_error
	{
	public:
		ChildrenNotAllowed() : std::runtime_error("Children are not allowed in this tag"){}
	};
}