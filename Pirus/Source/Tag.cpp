#include "Tag.h"
#include "Exceptions.h"
#include <algorithm>
#include <exception>

namespace Pirus
{
	Tag::Tag(const string& name, Pirus::ALLOW_CHILDREN allow_children)
		: m_name(name), 
		m_allow_children(allow_children == ALLOW_CHILDREN::YES?true:false), 
		m_attributes{}, 
		m_children{},
		m_text(),
		m_level{0}
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
		auto found = std::find_if(this->m_attributes[name].begin(), this->m_attributes[name].end(),[&](const auto& atr){ return atr.first == key;});

		if(found != this->m_attributes[name].end())
			return found->second;
		else
			throw Pirus::AttributeNotFound();
	}

	std::vector<string> Tag::get_attributes_names()
	{
		std::vector<string> names(this->m_attributes.size());
		std::transform(this->m_attributes.begin(), this->m_attributes.end(), names.begin(),[](const auto& v){ return v.first;});
		return names;
	}

	bool Tag::attribute_exists(const string& name, const string& key)
	{
		auto found = std::find_if(this->m_attributes[name].begin(), this->m_attributes[name].end(), [&] (const auto& atr) { return atr.first == key; });

		if (found != this->m_attributes[name].end())
			return true;
		else
			return false;
	}

	bool Tag::remove_attribute(const string& name, const string& key)
	{
		auto old_size = this->m_attributes[name].size();
		auto new_end = std::remove_if(this->m_attributes[name].begin(), this->m_attributes[name].end(), [&](const auto& v){ return v.first == key; });
		this->m_attributes[name].erase(new_end,this->m_attributes[name].end());
		return old_size != this->m_attributes[name].size();
	}

	void Tag::add_child(Tag&& child)
	{
		if(!this->children_allowed())
			throw Pirus::ChildNotAllowed();

		if(this->get_type_of_children() == Pirus::CHILD_TYPE::TEXT)
			throw Pirus::AlreadyHasDiffrentChildType();

		child.m_level = this->m_level + 1;
		child.update_children_level();
		this->m_children.emplace_back(child);

	}

	void Tag::add_child(const Tag& child)
	{
		this->add_child(Tag(child));
	}

	void Tag::add_child(const text & child)
	{
		if (!this->children_allowed())
			throw Pirus::ChildNotAllowed();

		if (this->get_type_of_children() == Pirus::CHILD_TYPE::TAG)
			throw Pirus::AlreadyHasDiffrentChildType();

		this->m_text = child;
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

	size_t Tag::get_level() const
	{
		return this->m_level;
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

	void Tag::prepare_name()
	{
	//tolower
		std::transform(this->m_name.begin(), this->m_name.end(), this->m_name.begin(), ::tolower);
		
	//trim
		size_t first = this->m_name.find_first_not_of(' ');
		size_t last = this->m_name.find_last_not_of(' ');
		this->m_name = this->m_name.substr(first, (last - first + 1));
	}

	void Tag::update_children_level()
	{
		if(this->m_children.size() == 0)
			return;

		for (auto& child : this->m_children)
		{
			child.m_level = this->m_level + 1;
			child.update_children_level();
		}
	}

	const string& Tag::get_name() const
	{
		return this->m_name;
	}

	std::ostream& operator<<(std::ostream& os, const Tag& tag)
	{
		//opentag
		os << "<" << tag.get_name();

		//print attributes
		for(const auto& attributes : tag.m_attributes)
		{
			os << " " << attributes.first << "=\"";
			for(const auto& attribute : attributes.second)
			{
				if(attribute.first == "")
					os << attribute.second;
				else
					os << attribute.first << ": " << attribute.second << ";";
			}
			os << "\"";
		}

		//close tag
		if (tag.children_allowed())
		{
			os << ">";
			//print children
			os << tag.get_text();
			for(const auto& child : tag.m_children)
			{
				os << child;
			}

			os << "</" << tag.get_name() << ">";
		}
		else
		{
			os << " />";
		}

		return os;
	}
}