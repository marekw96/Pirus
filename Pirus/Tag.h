#pragma once

#include "Definitions.h"
#include <optional>
#include <variant>
#include <vector>
#include <algorithm>

namespace Pirus
{
	class Tag
	{
	public:
		Tag(const Pirus::text& name, Pirus::ALLOW_CHILDREN allow_children);
		Tag(const Pirus::Tag&) = default;
		Tag(Pirus::Tag&&) = default;
		Pirus::Tag& operator=(const Pirus::Tag&) = default;
		Pirus::Tag& operator=(Pirus::Tag&&) = default;
	
		const Pirus::text& get_name() const;
		bool are_children_allowed() const;

		std::optional<Pirus::text> get_attribute(const Pirus::text& name) const;
		void set_attribute(const Pirus::text& attribute_name, const Pirus::text& value);
		void remove_attribute(const Pirus::text& attribute_name);
		std::vector<Pirus::text> get_attributes_names() const;
	
		void add_child(const Pirus::Tag& child);
		void add_child(const Pirus::text& child);
		const std::vector<std::variant<Pirus::Tag, Pirus::text>>& get_children() const;
		template<typename T>
		void remove_children_if(T condition);

		virtual ~Tag() = default;

	private:
		Pirus::text name;
		Pirus::ALLOW_CHILDREN allow_children;
		Pirus::attributes_map attributes;
		std::vector<std::variant<Pirus::Tag, Pirus::text>> children;
	};

	template<typename T>
	void Tag::remove_children_if(T condition)
	{
		auto deleter = std::remove_if(std::begin(children), std::end(children), condition);
		children.erase(deleter,std::end(children));
	}
}