#pragma once
#include <stdexcept>

namespace Pirus
{
	class ChildNotAllowed : public std::runtime_error
	{
	public:
		ChildNotAllowed() : runtime_error("Child not allowed"){}
	};

	class AttributeNotFound : public std::runtime_error
	{
	public:
		AttributeNotFound() : runtime_error("Attribute not found"){}
	};
}