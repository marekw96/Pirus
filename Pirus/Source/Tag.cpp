#include "Tag.h"
#include "Exceptions.h"
#include <algorithm>
#include <exception>
#include <sstream>
#include <utility>

namespace Pirus
{
	Tag::Tag(const string& name, Pirus::ALLOW_CHILDREN allow_children)
		: m_name(name), 
		m_allow_children(allow_children == ALLOW_CHILDREN::YES?true:false), 
		m_attributes{}, 
		m_children{},
		m_text()
	{
		this->prepare_name();
	}

	bool Tag::children_allowed() const
	{
		return this->m_allow_children;
	}

	void Tag::add_attribute(const string& name, const string& key, const string& value)
	{
		this->remove_attribute(name,key);
		this->m_attributes[name].push_back(attribute(key,value));
	}

	const std::vector<attribute>& Tag::get_attributes(const string& name)
	{
		return this->m_attributes[name];
	}

	const string& Tag::get_attribute(const string& name, const string& key)
	{
		auto found = std::find_if(std::cbegin(this->m_attributes[name]),
								 std::cend(this->m_attributes[name]),
								 [&](const auto& atr){ return atr.first == key;});

		if(found != std::cend(this->m_attributes[name]))
			return found->second;
		else
			throw Pirus::AttributeNotFound();
	}

	std::vector<string> Tag::get_attributes_names()
	{
		std::vector<string> names(this->m_attributes.size());
		std::transform(std::cbegin(this->m_attributes), std::cend(this->m_attributes), std::begin(names),[](const auto& v){ return v.first;});
		return names;
	}

	bool Tag::attribute_exists(const string& name, const string& key)
	{
		return std::cend(this->m_attributes[name]) != std::find_if(std::cbegin(this->m_attributes[name]),
																	std::cend(this->m_attributes[name]), 
																	[&] (const auto& atr) { return atr.first == key; });
	}

	bool Tag::remove_attribute(const string& name, const string& key)
	{
		auto new_end = std::remove_if(std::begin(this->m_attributes[name]), 
										std::end(this->m_attributes[name]), 
										[&](const auto& v){ return v.first == key; });

		if(new_end != std::end(this->m_attributes[name]))
		{
				this->m_attributes[name].erase(new_end, std::end(this->m_attributes[name]));
				return true;
		}
		return false;		
	}

	void Tag::add_child(Tag&& child)
	{
		if(!this->children_allowed())
			throw Pirus::ChildNotAllowed();

		this->m_children.emplace_back(std::move(child));
	}

	void Tag::add_child(const Tag& child)
	{
		this->add_child(Tag(child));
	}

	void Tag::add_child(const text & child)
	{
		if (!this->children_allowed())
			throw Pirus::ChildNotAllowed();

		Tag tag("plain text", Pirus::ALLOW_CHILDREN::NO);
		tag.m_text = child;
		this->add_child(std::move(tag));
	}

	std::vector<Pirus::Tag>::size_type Tag::count_children() const
	{
		return this->m_children.size();
	}

	void Tag::clear_children()
	{
		this->m_children.clear();
		this->m_text.clear();
	}

	const text& Tag::get_text() const
	{
		auto find = std::find_if(std::cbegin(this->m_children), 
								std::cend(this->m_children), 
								[](const auto& child){ return child.get_name() == "plain text"; });

		if(find != std::cend(this->m_children))
			return find->get_text();

		return this->m_text;
	}

	std::vector<Pirus::Tag>& Tag::get_children()
	{
		return this->m_children;
	}

	const std::vector<Pirus::Tag>& Tag::get_children() const
	{
		return this->m_children;
	}


	Pirus::CHILD_TYPE Tag::get_type_of_children() const
	{
		if(!this->children_allowed())
		{
			return Pirus::CHILD_TYPE::NOT_ALLOWED;
		}
		
		if (this->get_text().size() == 0)
		{
			if(this->count_children() == 0)
			{
				return Pirus::CHILD_TYPE::NONE;
			}
			else
			{
				return Pirus::CHILD_TYPE::TAG;
			}
		}
		else
		{
			return Pirus::CHILD_TYPE::TEXT;
		}
	}

	void Tag::clear()
	{
		this->m_name.clear();
		this->m_attributes.clear();
		this->clear_children();
	}

	text Tag::to_text(size_t level) const
	{
		std::stringstream output;
		
		for(size_t it = 0; it < level; ++it)
			output << '\t';

		if (this->get_name() == "plain text")
		{
			output << this->get_text();
			return output.str();;
		}

		this->open_tag_to_stream(output,level);
		this->children_to_stream(output,level);
		this->close_tag_to_stream(output,level);

		return output.str();
	}

	const string& Tag::get_name() const
	{
		return this->m_name;
	}

	std::ostream& operator<<(std::ostream& os, const Tag& tag)
	{
		return os << tag.to_text();
	}

	void Tag::prepare_name()
	{
		std::transform(std::cbegin(this->m_name), std::cend(this->m_name), std::begin(this->m_name), ::tolower);
		
		size_t first = this->m_name.find_first_not_of(" \t");
		size_t last = this->m_name.find_last_not_of(" \t");
		this->m_name = this->m_name.substr(first, (last - first + 1));
	}

	void Tag::open_tag_to_stream(std::ostream& stream, size_t level) const
	{
		stream << "<" << this->get_name();

		for (const auto& attributes : this->m_attributes)
		{
			stream << " " << attributes.first << "=\"";
			for (const auto& attribute : attributes.second)
			{
				if (attribute.first == "")
					stream << attribute.second;
				else
					stream << attribute.first << ": " << attribute.second << ";";
			}
			stream << "\"";
		}

		if (this->children_allowed())
			stream << ">";
		else
			stream << " />";
	}

	void Tag::children_to_stream(std::ostream& stream, size_t level) const
	{
		if (this->children_allowed())
		{
			for (const auto& child : this->m_children)
			{
				stream << '\n' << child.to_text(level + 1);
			}

			if (this->get_type_of_children() != CHILD_TYPE::NONE)
			{
				stream << '\n';
				for (size_t it = 0; it < level; ++it)
					stream << '\t';
			}
		}
	}

	void Tag::close_tag_to_stream(std::ostream & stream, size_t level) const
	{
		if (this->children_allowed())
			stream << "</" << this->get_name() << ">";
	}
}