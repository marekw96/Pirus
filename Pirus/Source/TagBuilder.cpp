#include "TagBuilder.h"
#include "Exceptions.h"

Pirus::TagBuilder::TagBuilder() :
	m_name{},
	m_attributes{},
	m_allow_children(true)
{}

Pirus::TagBuilder & Pirus::TagBuilder::set_name(const text& name)
{
	this->m_name = name;
	return *this;
}

Pirus::TagBuilder & Pirus::TagBuilder::set_name(text&& name)
{
	this->m_name = std::move(name);
	return *this;
}

Pirus::TagBuilder & Pirus::TagBuilder::allow_children(bool allow)
{
	this->m_allow_children = allow;
	return *this;
}

Pirus::TagBuilder & Pirus::TagBuilder::allow_children(Pirus::ALLOW_CHILDREN allow)
{
	this->m_allow_children = (allow == Pirus::ALLOW_CHILDREN::YES? true : false);
	return *this;
}

Pirus::TagBuilder & Pirus::TagBuilder::set_attribute(const text & name, const text & value)
{
	this->m_attributes[name] = value;
	return *this;
}

Pirus::TagBuilder & Pirus::TagBuilder::clear()
{
	this->m_allow_children = true;
	this->m_attributes.clear();
	this->m_name.clear();

	return *this;
}

Pirus::Tag Pirus::TagBuilder::build()
{
	if(this->m_name.empty())
		throw Pirus::NameNotSet();

	Pirus::Tag tag(this->m_name, this->m_allow_children ? Pirus::ALLOW_CHILDREN::YES : Pirus::ALLOW_CHILDREN::NO);
	
	tag.m_attributes = this->m_attributes;

	return tag;
}
