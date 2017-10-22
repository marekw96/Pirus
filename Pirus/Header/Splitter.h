#pragma once
#include "Constants.h"
#include "Exceptions.h"
#include "Fragment.h"
#include "Tag.h"
#include "TagBuilder.h"
#include <vector>
#include <sstream>

namespace Pirus
{
	class Splitter
	{
	public:
		Splitter();
		void set_content(const text& content);
		void set_content(text&& content);
		const text& get_content() const;
		
		std::vector<Pirus::Fragment>& parse_to_fragments();
		std::vector<Pirus::Tag>& parse_to_tags();

		std::vector<Pirus::Tag>& operator()(const text& code);

		const std::vector<Pirus::Fragment>& get_fragments() const;
		const std::vector<Pirus::Tag>& get_tags() const;

		std::vector<Pirus::Fragment>& get_fragments();
		std::vector<Pirus::Tag>& get_tags();

		void clear();

	private:
		FRAGMENT_TYPE m_status;
		std::vector<Pirus::Fragment> m_fragments;
		std::vector<Pirus::Tag> m_tags;
		size_t m_level;
		size_t m_max_level;
		std::stringstream m_stream;
		bool m_escape;
		text m_content;

		Pirus::Fragment make_fragment();

		template <typename T>
		void parse_text(T sign);
		template <typename T>
		void parse_tag(T sign);
		template <typename T>
		void parse_attribute_name(T sign);
		template <typename T>
		void parse_attribute_value(T sign);

		void parse_tag_attribute_name(TagBuilder& builder, std::pair<text, text>& attribute,const Fragment& fragment);
		void parse_tag_attribute_value(TagBuilder& builder, std::pair<text, text>& attribute,const Fragment& fragment);
		Tag* parse_tag_tag(TagBuilder& builder, const Fragment& fragment, std::vector<Tag*>& parents, Tag* parent, size_t& last_level);
		Tag* parse_tag_close(TagBuilder& builder, const Fragment& fragment, std::vector<Tag*>& parents, Tag* parent, size_t& last_level);
		Tag* parse_tag_text(TagBuilder& builder, const Fragment& fragment, std::vector<Tag*>& parents, Tag* parent, size_t& last_level);
	};
}