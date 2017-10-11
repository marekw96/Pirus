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

	class AlreadyHasDiffrentChildType : public std::runtime_error
	{
	public:
		AlreadyHasDiffrentChildType() : runtime_error("Tag already has diffrent type of child"){}
	};
}