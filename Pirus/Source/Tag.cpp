#include "Tag.h"
#include "Exceptions.h"
#include <algorithm>
#include <exception>
#include <sstream>

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

		Tag tag("plain text", Pirus::ALLOW_CHILDREN::YES);
		tag.m_text = child;
		this->add_child(tag);
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
		auto find = std::find_if(this->m_children.begin(), this->m_children.end(), [](const auto& child){ return child.get_name() == "plain text"; });
		if(find != this->m_children.end())
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

		output << "<" << this->get_name();

		for (const auto& attributes : this->m_attributes)
		{
			output << " " << attributes.first << "=\"";
			for (const auto& attribute : attributes.second)
			{
				if (attribute.first == "")
					output << attribute.second;
				else
					output << attribute.first << ": " << attribute.second << ";";
			}
			output << "\"";
		}

		if (this->children_allowed())
		{
			output << ">";

			for (const auto& child : this->m_children)
			{
				output << '\n' << child.to_text(level+1);
			}

			if (this->get_type_of_children() != CHILD_TYPE::NONE)
			{
				output << '\n';
				for (size_t it = 0; it < level; ++it)
					output << '\t';
			}
			output << "</" << this->get_name() << ">";
		}
		else
		{
			output << " />";
		}

		return output.str();
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
		os << tag.to_text();

		return os;
	}
}