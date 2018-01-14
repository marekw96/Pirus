#pragma once

#include "Definitions.h"
#include "Tag.h"

namespace Pirus
{
	class TagFormatter
	{
	public:
		TagFormatter(const Pirus::Tag& tag);

		const Pirus::text& format() const;

		virtual ~TagFormatter() = default;

	private:
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

	};

}
