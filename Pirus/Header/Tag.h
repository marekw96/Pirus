#pragma once
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <iostream>

namespace Pirus
{
	class Tag
	{
	public:
		explicit Tag(const std::string& name, bool contains_content);
		Tag(const Tag&) = default;
		Tag(Tag&&) = default;
		~Tag() = default;
		Tag& operator=(const Tag&) = default;
		Tag& operator=(Tag&&) = default;
		const std::string& get_name() const;
		bool children_allowed() const;
		void add_attribute(const std::string& name, const std::string& key, const std::string& value);
		const std::vector<std::pair<std::string,std::string>>& get_attributes(const std::string& name);
		const std::string& get_attribute(const std::string& name, const std::string& key);
		const std::vector<std::string> get_attributes_names();
		bool attribute_exists(const std::string& name, const std::string& key);
		bool remove_attribute(const std::string& name, const std::string& key);
		void add_child(Tag&& child);
		std::vector<Pirus::Tag>::size_type count_children();
		void clear_children();

		void clear();
	
		friend std::ostream& operator<<(std::ostream& os, const Tag& tag);
	private:
		uint8_t m_allow_children:1;
		std::string m_name;
		std::unordered_map<std::string,std::vector<std::pair<std::string,std::string>>> m_attributes;
		std::vector<Pirus::Tag> m_children;
		std::string m_text;
		
		void prepare_name();
	};
}