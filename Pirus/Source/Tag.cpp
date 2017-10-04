#include "Tag.h"
#include "Exceptions.h"
#include <algorithm>
#include <exception>

namespace Pirus
{
	Tag::Tag(const string& name, bool contains_content) 
		: m_name(name), 
		m_allow_children(contains_content), 
		m_attributes{}, 
		m_children{},
		m_text()
	{
		this->prepare_name();
	}

	bool Tag::children_allowed() const
	{
		return this->m_allow_children ? true : false;
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
		auto& vector_of_attributes = this->m_attributes[name];

		for(const auto& attribute : vector_of_attributes)
		{
			if(attribute.first == key)
				return attribute.second;
		}
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
		const auto& attributes = this->get_attributes(name);
		for(const auto& var : attributes)
		{
			if(var.first == key)
				return true;
		}
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
		if(this->children_allowed())
			this->m_children.emplace_back(child);
		else
			throw Pirus::ChildNotAllowed();
	}

	std::vector<Pirus::Tag>::size_type Tag::count_children()
	{
		return this->m_children.size();
	}

	void Tag::clear_children()
	{
		this->m_children.clear();
		this->m_text.clear();
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