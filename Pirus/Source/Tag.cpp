#include "..\Tag.h"

#include <algorithm>

Pirus::Tag::Tag(const Pirus::text& name, Pirus::ALLOW_CHILDREN allow_children)
	: name{name}, allow_children{allow_children}
{}

const Pirus::text& Pirus::Tag::get_name() const
{
	return name;
}

bool Pirus::Tag::are_children_allowed() const
{
	return allow_children == Pirus::ALLOW_CHILDREN::YES ? true : false;
}

std::optional<Pirus::text> Pirus::Tag::get_attribute(const Pirus::text& attribute_name) const
{
	auto find = std::find_if(std::cbegin(attributes), 
							std::cend(attributes), 
							[&attribute_name](const auto& attribute){ return attribute.first == attribute_name;});
	
	if(find != std::cend(attributes))
		return find->second;
	return std::nullopt;
}

void Pirus::Tag::set_attribute(const Pirus::text& attribute_name, const Pirus::text& value)
{
	attributes[attribute_name] = value;
}

void Pirus::Tag::remove_attribute(const Pirus::text& attribute_name)
{
	attributes.erase(attribute_name);
}

std::vector<Pirus::text> Pirus::Tag::get_attributes_names() const
{
	std::vector<Pirus::text> names;
	for (const auto& attribute : attributes)
	{
		names.emplace_back(attribute.first);
	}

	return names;
}
