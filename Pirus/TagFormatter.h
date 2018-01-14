#pragma once

#include <sstream>

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
		void generate_output();
		void build_front(const Pirus::Tag& tag);
		void build_end_of_single_tag();
		Pirus::text transform_tag_name(std::wstring_view tag_name);

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
