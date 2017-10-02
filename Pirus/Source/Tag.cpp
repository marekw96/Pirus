#include "../Header/Tag.h"
#include <algorithm>
#include <exception>

namespace Pirus
{
	Tag::Tag(const std::string & name, bool contains_content) : m_name(name), m_allow_content(contains_content), m_attributes{}
	{
		this->prepare_name();
	}

	bool Tag::contains_content() const
	{
		return this->m_allow_content ? true : false;
	}

	void Tag::add_attribute(const std::string& name, const std::string& key, const std::string& value)
	{
		this->m_attributes[name].push_back(std::pair<std::string,std::string>(key,value));
	}

	const std::vector<std::pair<std::string, std::string>>& Tag::get_attributes(const std::string& name)
	{
		return this->m_attributes[name];
	}

	const std::string& Tag::get_attribute(const std::string& name, const std::string& key)
	{
		auto& vector_of_attributes = this->m_attributes[name];
		for each (const auto& attribute in vector_of_attributes)
		{
			if(attribute.first == key)
				return attribute.second;
		}
		throw std::exception("Not found");
	}

	const std::vector<std::string> Tag::get_attributes_names()
	{
		std::vector<std::string> names;
		names.reserve(this->m_attributes.size());
		for each (auto& attribute in this->m_attributes)
		{
			names.emplace_back(attribute.first);
		}
		return names;
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

	const std::string& Tag::get_name() const
	{
		return this->m_name;
	}

	std::ostream& operator<<(std::ostream& os, const Tag& tag)
	{
		//opentag
		os << "<" << tag.get_name();

		//print attributes
		for each(auto& attributes in tag.m_attributes)
		{
			os << " " << attributes.first << "=\"";
			for each(auto& attribute in attributes.second)
			{
				os << attribute.first << ": " << attribute.second << ";";
			}
			os << "\"";
		}

		//close tag
		if (tag.contains_content())
		{
			os << "></" << tag.get_name() << ">";
		}
		else
		{
			os << " />";
		}

		return os;
	}
}