#include "..\TagFormatter.h"

#include <cctype>
#include <algorithm>


Pirus::TagFormatter::TagFormatter(const Pirus::Tag& tag)
	: tag_to_format(tag),
	indention{Pirus::DefaultFormatterOptions::indention},
	new_line{Pirus::DefaultFormatterOptions::new_line},
	open_tag{Pirus::DefaultFormatterOptions::open_tag},
	close_tag{Pirus::DefaultFormatterOptions::close_tag},
	single_tag_close_mark{Pirus::DefaultFormatterOptions::single_tag_close_mark},
	container_tag_close_mark{Pirus::DefaultFormatterOptions::container_tag_close_mark},
	attribute_restraint_mark{Pirus::DefaultFormatterOptions::attribute_restraint_mark},
	format_tag_name{Pirus::DefaultFormatterOptions::format_tag_name},
	format_attribute_name{Pirus::DefaultFormatterOptions::format_attribute_name},
	format_attribute_value{Pirus::DefaultFormatterOptions::format_attribute_value}
{}

const Pirus::text & Pirus::TagFormatter::to_text()
{
	if(generated_output.empty())
		generate_output();

	return generated_output;
}

void Pirus::TagFormatter::generate_output()
{
	build_front(tag_to_format);
	if (!tag_to_format.are_children_allowed())
	{
		build_end_of_single_tag();
	}

	generated_output = builder.str();
}

void Pirus::TagFormatter::build_front(const Pirus::Tag& tag)
{
	builder << open_tag;
	builder << transform_tag_name(tag.get_name());
	builder << " ";
}

void Pirus::TagFormatter::build_end_of_single_tag()
{
	builder << single_tag_close_mark;
	builder << close_tag;
}

Pirus::text Pirus::TagFormatter::transform_tag_name(std::wstring_view tag_name)
{
	Pirus::text txt(tag_name);
	switch (format_tag_name)
	{
		case Pirus::FORMAT_TEXT::SKIP:
		break;
		case Pirus::FORMAT_TEXT::LOWERCASE:
			std::transform(std::begin(txt), std::end(txt), std::begin(txt), std::tolower);
		break;
		case Pirus::FORMAT_TEXT::CAPITAL:
			std::transform(std::begin(txt), std::end(txt), std::begin(txt), std::toupper);
		break;
		case Pirus::FORMAT_TEXT::FIRST_LETTER_CAPITAL:
		{
			if (txt.size() > 1)
			{
				txt[0] = std::toupper(txt[0]);
			}
		}
		break;
	}

	return txt;
}
