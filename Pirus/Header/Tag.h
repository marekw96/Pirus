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
		bool contains_content() const;
		void add_attribute(const std::string& name, const std::string& key, const std::string& value);
		const std::vector<std::pair<std::string,std::string>>& get_attributes(const std::string& name);
		const std::string& get_attribute(const std::string& name, const std::string& key);
		const std::vector<std::string> get_attributes_names();
		bool attribute_exists(const std::string& name, const std::string& key);
		bool remove_attribute(const std::string& name, const std::string& key);
	
		friend std::ostream& operator<<(std::ostream& os, const Tag& tag);
	private:
		uint8_t m_allow_content:1;
		std::string m_name;
		std::unordered_map<std::string,std::vector<std::pair<std::string,std::string>>> m_attributes;
		
		void prepare_name();
	};
}