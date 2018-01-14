#include "..\TagFormatter.h"


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

const Pirus::text & Pirus::TagFormatter::format() const
{
	return generated_output;
}
