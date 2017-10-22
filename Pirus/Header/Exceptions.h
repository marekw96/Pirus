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

	class FragmentsNotFound : public std::runtime_error
	{
	public:
		FragmentsNotFound() : runtime_error("Fragments not found"){}
	};

	class ContentNotFound : public std::runtime_error
	{
	public:
		ContentNotFound() : runtime_error("Content not found") {}
	};

	class NameNotSet : public std::runtime_error
	{
	public:
		NameNotSet() : runtime_error("Name of tag not set"){}
	};
}