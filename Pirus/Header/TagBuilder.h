#pragma once

#include "Constants.h"
#include "Tag.h"
#include <unordered_map>

namespace Pirus
{
	class TagBuilder
	{
	public:
		TagBuilder();
		TagBuilder& set_name(const text& name);
		TagBuilder& set_name(text&& name);
		TagBuilder& allow_children(bool allow);
		TagBuilder& allow_children(Pirus::ALLOW_CHILDREN allow);
		TagBuilder& set_attribute(const text& name, const text& value);
		TagBuilder& clear();

		Tag build();


		template <typename T>
		TagBuilder& allow_children(T allow) = delete;
	private:
		text m_name;
		std::unordered_map<text,text> m_attributes;
		bool m_allow_children;
	};
}
