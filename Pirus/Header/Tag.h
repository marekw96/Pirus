#pragma once
#include "Constants.h"
#include <unordered_map>
#include <utility>
#include <vector>
#include <iostream>
#include <sstream>

namespace Pirus
{
	class Tag
	{
	public:
		explicit Tag(const text& name, Pirus::ALLOW_CHILDREN allow_children);
		Tag(const Tag&) = default;
		Tag(Tag&&) = default;
		~Tag() = default;
		Tag& operator=(const Tag&) = default;
		Tag& operator=(Tag&&) = default;
		const text& get_name() const;
		bool children_allowed() const;
		void add_attribute(const text& name, const text& value);
		const text& get_attribute(const text& key) const;
		std::vector<text> get_attributes_names() const;
		bool attribute_exists(const text& name);
		bool remove_attribute(const text& name);
		void add_child(Tag&& child);
		void add_child(const Tag& child);
		void add_child(const text& child);
		std::vector<Pirus::Tag>::size_type count_children() const;
		void clear_children();
		const text& get_text() const;
		std::vector<Pirus::Tag>& get_children();
		const std::vector<Pirus::Tag>& get_children() const;
		Pirus::CHILD_TYPE get_type_of_children() const;
		void clear();
		text to_text(size_t level = 0) const;
	
		friend std::ostream& operator<<(std::ostream& os, const Tag& tag);
	private:
		bool m_allow_children;
		text m_name;
		std::unordered_map<text, text> m_attributes;
		std::vector<Pirus::Tag> m_children;
		text m_text;
		
		void prepare_name();

		void open_tag_to_stream(std::ostream& stream, size_t level) const;
		void children_to_stream(std::ostream& stream, size_t level) const;
		void close_tag_to_stream(std::ostream& stream, size_t level) const;
		void attributes_to_stream(std::ostream& stream) const;
	};
}