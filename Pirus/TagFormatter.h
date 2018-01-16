#pragma once

#include <sstream>
#include <utility>
#include <queue>

#include "Definitions.h"
#include "Tag.h"

namespace Pirus
{
	class TagFormatter
	{
	public:
		TagFormatter(const Pirus::Tag& tag);

		const Pirus::text& to_text();

		virtual ~TagFormatter() = default;

	private:
		void generate_output(const Pirus::Tag& tag);
		std::pair<std::queue<const Pirus::Tag*>, unsigned> build_open_tags(const Pirus::Tag& tag);
		void build_close_tags(std::queue<const Pirus::Tag*>& to_close, unsigned level);
		void build_new_line_and_indetions(unsigned level);
		void build_front(const Pirus::Tag& tag);
		void build_attributes(const Pirus::Tag& tag);
		void build_end_of_single_tag();
		void build_close_of_container_tag();
		void build_end_of_container_tag(const Pirus::Tag& tag);
		Pirus::text transform_by(std::wstring_view tag_name, Pirus::FORMAT_TEXT format_type);

		const Pirus::Tag& tag_to_format;

		Pirus::text indention;
		Pirus::text new_line;
		Pirus::text open_tag;
		Pirus::text close_tag;
		Pirus::text single_tag_close_mark;
		Pirus::text container_tag_close_mark;
		Pirus::text attribute_restraint_mark;

		Pirus::FORMAT_TEXT format_tag_name;
		Pirus::FORMAT_TEXT format_attribute_name;
		Pirus::FORMAT_TEXT format_attribute_value;

		Pirus::text generated_output;
		std::wstringstream builder;
	};

}
