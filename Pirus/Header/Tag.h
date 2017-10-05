#pragma once
#include "Constants.h"
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
		explicit Tag(const string& name, bool contains_content);
		Tag(const Tag&) = default;
		Tag(Tag&&) = default;
		~Tag() = default;
		Tag& operator=(const Tag&) = default;
		Tag& operator=(Tag&&) = default;
		const string& get_name() const;
		bool children_allowed() const;
		void add_attribute(const string& name, const string& key, const string& value);
		const std::vector<attribute>& get_attributes(const string& name);
		const string& get_attribute(const string& name, const string& key);
		std::vector<string> get_attributes_names();
		bool attribute_exists(const string& name, const string& key);
		bool remove_attribute(const string& name, const string& key);
		void add_child(Tag&& child);
		void add_child(const Tag& child);
		std::vector<Pirus::Tag>::size_type count_children() const;
		void clear_children();
		void set_text(const text& t);
		const text& get_text() const;
		std::vector<Pirus::Tag>& get_children();
		const std::vector<Pirus::Tag>& get_children() const;

		void clear();
	
		friend std::ostream& operator<<(std::ostream& os, const Tag& tag);
	private:
		bool m_allow_children;
		string m_name;
		std::unordered_map<string,std::vector<attribute>> m_attributes;
		std::vector<Pirus::Tag> m_children;
		text m_text;
		
		void prepare_name();
	};
}