#include "Splitter.h"
#include <utility>
#include <algorithm>

#include <iostream>

Pirus::Splitter::Splitter() :
	m_status(FRAGMENT_TYPE::TEXT),
	m_fragments{},
	m_tags{},
	m_level{},
	m_stream{},
	m_escape{false},
	m_content{},
	m_max_level{0}
{}

void Pirus::Splitter::set_content(const text& content)
{
	this->clear();
	this->m_content = content;
}

void Pirus::Splitter::set_content(text&& content)
{
	this->clear();
	this->m_content = std::move(content);
}

const Pirus::text& Pirus::Splitter::get_content() const
{
	return this->m_content;
}

std::vector<Pirus::Fragment>& Pirus::Splitter::parse_to_fragments()
{
	if(this->m_content.size() == 0)
		throw Pirus::ContentNotFound();
	
	if(this->m_fragments.size() > 0)//TODO: throw exception?
		this->set_content(this->m_content);

	for (auto sign : this->m_content)
	{
		switch (this->m_status)
		{
			case FRAGMENT_TYPE::TEXT:
			this->parse_text(sign);
			break;
			case FRAGMENT_TYPE::TAG:
			this->parse_tag(sign);
			break;
			case FRAGMENT_TYPE::ATTRIBUTE_NAME:
			this->parse_attribute_name(sign);
			break;
			case FRAGMENT_TYPE::ATTRIBUTE_VALUE:
			this->parse_attribute_value(sign);
			break;
		}
	}

	return this->m_fragments;
}

std::vector<Pirus::Tag>& Pirus::Splitter::parse_to_tags()
{
	if (this->m_fragments.empty())
		throw Pirus::ContentNotFound();

	auto builder = Pirus::TagBuilder();

	std::pair<text,text> attribute;

	Tag* parent = nullptr;

	size_t last_level = 0U;

	std::vector<Tag*> parents(this->m_max_level, nullptr);

	for (const auto& fragment : this->m_fragments)
	{
		switch (fragment.type)
		{
			case FRAGMENT_TYPE::TAG :
				parent = this->parse_tag_tag(builder, fragment, parents,parent, last_level);
			break;
			case FRAGMENT_TYPE::ATTRIBUTE_NAME :
				this->parse_tag_attribute_name(builder,attribute,fragment);
			break;
			case FRAGMENT_TYPE::ATTRIBUTE_VALUE :
				this->parse_tag_attribute_value(builder, attribute, fragment);
			break;
			case FRAGMENT_TYPE::TEXT :
				parent = this->parse_tag_text(builder, fragment, parents, parent, last_level);
			break;
			case FRAGMENT_TYPE::CLOSE_TAG :
				parent = this->parse_tag_close(builder, fragment, parents, parent, last_level);
			break;
		}
	}

	return this->m_tags;
}

std::vector<Pirus::Tag>& Pirus::Splitter::operator()(const text & code)
{
	this->set_content(code);
	this->parse_to_fragments();
	this->parse_to_tags();

	return this->m_tags;
}

const std::vector<Pirus::Tag>& Pirus::Splitter::get_tags() const
{
	return this->m_tags;
}

std::vector<Pirus::Fragment>& Pirus::Splitter::get_fragments()
{
	return this->m_fragments;
}

std::vector<Pirus::Tag>& Pirus::Splitter::get_tags()
{
	return this->m_tags;
}

void Pirus::Splitter::clear()
{
	this->m_status = FRAGMENT_TYPE::TEXT;
	this->m_escape = false;
	this->m_level = 0;
	this->m_fragments.clear();
	this->m_tags.clear();
	this->m_stream.str().clear();
	this->m_content.clear();
}

const std::vector<Pirus::Fragment>& Pirus::Splitter::get_fragments() const
{
	return this->m_fragments;
}

Pirus::Fragment Pirus::Splitter::make_fragment()
{
	if(this->m_stream.str().size() == 0)
		return Pirus::Fragment();

	Pirus::Fragment f;
	f.type = this->m_status;
	f.value = this->m_stream.str();
	f.level = this->m_level;
	this->m_stream.str("");
	
	if(f.type != FRAGMENT_TYPE::TEXT)
	{
		if(f.value[0] == '/')
		{
			f.type = FRAGMENT_TYPE::CLOSE_TAG;
			
			if(f.value.size() > 1)
			{
				f.value.erase(0,1);
			}

			f.level = --this->m_level;
		}
		if (f.type == FRAGMENT_TYPE::TAG)
		{
			++this->m_level;
		}
		if (f.type == FRAGMENT_TYPE::ATTRIBUTE_VALUE)
		{
			if(f.value[0] == '"')
				f.value.erase(0, 1);
		}
		else if (f.type == FRAGMENT_TYPE::ATTRIBUTE_NAME)
		{
		}
	}

	this->m_max_level = std::max(this->m_max_level, this->m_level);

	return f;
}

void Pirus::Splitter::parse_tag_attribute_name(TagBuilder& builder, std::pair<text, text>& attribute, const Fragment& fragment)
{
	attribute.first = fragment.value;
}

void Pirus::Splitter::parse_tag_attribute_value(TagBuilder& builder, std::pair<text, text>& attribute, const Fragment& fragment)
{
	attribute.second = fragment.value;
	builder.set_attribute(attribute.first, attribute.second);
}

Pirus::Tag * Pirus::Splitter::parse_tag_tag(TagBuilder& builder, const Fragment& fragment, std::vector<Tag*>& parents, Tag* parent, size_t& last_level)
{
	if (builder.has_name())
	{
		if (last_level == 0)
		{
			this->m_tags.emplace_back(builder.build());
			parent = &this->m_tags.back();
		}
		else
		{
			parent->add_child(builder.build());
			parent = &parent->get_children().back();
		}

		parents[last_level] = parent;
		last_level = fragment.level;
		builder.clear();
	}
	builder.set_name(fragment.value);

	return parent;
}

Pirus::Tag * Pirus::Splitter::parse_tag_close(TagBuilder & builder, const Fragment & fragment, std::vector<Tag*>& parents, Tag * parent, size_t & last_level)
{
	if (fragment.value == "/")
	{
		builder.allow_children(ALLOW_CHILDREN::NO);
		if (last_level == 0)
		{
			this->m_tags.emplace_back(builder.build());
		}
		else
		{
			parent->add_child(builder.build());
		}
	}
	else
	{
		if (fragment.level != 0)
			parent = parents[fragment.level - 1];
		else
			parent = parents[0];
	}

	builder.clear();

	return parent;
}

Pirus::Tag * Pirus::Splitter::parse_tag_text(TagBuilder & builder, const Fragment & fragment, std::vector<Tag*>& parents, Tag * parent, size_t & last_level)
{
	if (builder.has_name())
	{
		auto tag = builder.build();
		tag.add_child(fragment.value);
		if (last_level == 0)
		{
			this->m_tags.emplace_back(std::move(tag));
		}
		else
		{
			parent->add_child(std::move(tag));
		}
		last_level = fragment.level;
		builder.clear();
	}
	else
	{
		auto tag = Tag("plain text", ALLOW_CHILDREN::YES);
		tag.add_child(fragment.value);
		if (last_level == 0)
		{
			this->m_tags.emplace_back(std::move(tag));
		}
		else
		{
			parent->add_child(std::move(tag));
		}
		last_level = fragment.level;
		builder.clear();
	}

	return parent;
}

template<typename T>
void Pirus::Splitter::parse_text(T sign)
{
	if (sign == '<')
	{
		if (this->m_stream.str().size() > 0)
			this->m_fragments.emplace_back(this->make_fragment());
		this->m_status = FRAGMENT_TYPE::TAG;
	}
	else
	{
		this->m_stream << sign;
	}
}

template<typename T>
void Pirus::Splitter::parse_tag(T sign)
{
	if (sign == '>')
	{
		if (this->m_stream.str().size() > 0)
			this->m_fragments.emplace_back(this->make_fragment());
		this->m_status = FRAGMENT_TYPE::TEXT;
	}
	else if (sign == ' ')
	{
		if (this->m_stream.str().size() > 0)
			this->m_fragments.emplace_back(this->make_fragment());
		this->m_status = FRAGMENT_TYPE::ATTRIBUTE_NAME;
	}
	else
	{
		this->m_stream << sign;
	}
}

template<typename T>
void Pirus::Splitter::parse_attribute_name(T sign)
{
	if (sign == '=')
	{
		if (this->m_stream.str().size() > 0)
			this->m_fragments.emplace_back(this->make_fragment());

		this->m_status = FRAGMENT_TYPE::ATTRIBUTE_VALUE;
		this->m_escape = true;
	}
	else if (sign == '>')
	{
		if (this->m_stream.str().size() > 0)
			this->m_fragments.emplace_back(this->make_fragment());

		this->m_status = FRAGMENT_TYPE::TEXT;
	}
	else if (sign == '/')
	{
		if (this->m_stream.str().size() > 0)
			this->m_fragments.emplace_back(this->make_fragment());

		this->m_stream << sign;
		this->m_status = FRAGMENT_TYPE::TAG;
	}
	else
	{
		this->m_stream << sign;
	}
}

template<typename T>
void Pirus::Splitter::parse_attribute_value(T sign)
{
	if (sign == '"' && this->m_escape == false)
	{
		if(this->m_stream.str().size() > 0)
			this->m_fragments.emplace_back(this->make_fragment());
		this->m_status = FRAGMENT_TYPE::TAG;
	}
	else
	{
		this->m_stream << sign;
	}
	this->m_escape = false;
}
