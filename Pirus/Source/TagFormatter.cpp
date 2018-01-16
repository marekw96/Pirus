#include "..\TagFormatter.h"

#include <cctype>
#include <algorithm>

Pirus::TagFormatter::TagFormatter(const Pirus::Tag& tag)
	: tag_to_format(tag),
	indention{ L"       " },
	new_line{ L"\n" },
	open_tag{ L"<" },
	close_tag{ L">" },
	single_tag_close_mark{ L"/" },
	container_tag_close_mark{ L"/" },
	attribute_restraint_mark{ L"\"" },
	format_tag_name{ Pirus::FORMAT_TEXT::LOWERCASE },
	format_attribute_name{ Pirus::FORMAT_TEXT::LOWERCASE },
	format_attribute_value{ Pirus::FORMAT_TEXT::SKIP }
{}

const Pirus::text & Pirus::TagFormatter::to_text()
{
	if(generated_output.empty())
		generate_output(tag_to_format);

	return generated_output;
}

void Pirus::TagFormatter::generate_output(const Pirus::Tag& tag)
{
	auto to_close_and_level = build_open_tags(tag);
	build_close_tags(to_close_and_level.first, to_close_and_level.second);
	
	generated_output = builder.str();
}

std::pair<std::queue<const Pirus::Tag*>, unsigned> Pirus::TagFormatter::build_open_tags(const Pirus::Tag& tag)
{
	std::queue<const Pirus::Tag*> to_print;
	std::queue<const Pirus::Tag*> to_close;
	unsigned level = 0;
	const Pirus::Tag* actual = nullptr;

	to_print.push(&tag);

	while (to_print.size())
	{
		actual = to_print.front();
		to_print.pop();

		build_new_line_and_indetions(level++);
		build_front(*actual);
		build_attributes(*actual);

		if (!actual->are_children_allowed())
		{
			build_end_of_single_tag();
		}
		else
		{
			build_close_of_container_tag();

			for (const auto& child : actual->get_children())
				to_print.push(&child);

			to_close.push(actual);
		}
	}

	return std::make_pair(to_close, level);
}

void Pirus::TagFormatter::build_close_tags(std::queue<const Pirus::Tag*>& to_close, unsigned level)
{
	const Pirus::Tag* actual = nullptr;

	while (to_close.size())
	{
		actual = to_close.front();
		to_close.pop();

		if (--level == 1)
			builder << new_line;
		else
			build_new_line_and_indetions(level);

		build_end_of_container_tag(*actual);
	}
}

void Pirus::TagFormatter::build_new_line_and_indetions(unsigned level)
{
	if (level == 0)
		return;

	builder << new_line;
	for (unsigned i = 0; i < level; ++i)
		builder << indention;
}

void Pirus::TagFormatter::build_front(const Pirus::Tag& tag)
{
	builder << open_tag;
	builder << transform_by(tag.get_name(), format_tag_name);
}

void Pirus::TagFormatter::build_end_of_single_tag()
{
	builder << L" ";
	builder << single_tag_close_mark;
	builder << close_tag;
}

void Pirus::TagFormatter::build_close_of_container_tag()
{
	builder << close_tag;
}

void Pirus::TagFormatter::build_end_of_container_tag(const Pirus::Tag& tag)
{
	builder << open_tag;
	builder << container_tag_close_mark;
	builder << transform_by(tag.get_name(), format_tag_name);
	builder << close_tag;
}

void Pirus::TagFormatter::build_attributes(const Pirus::Tag& tag)
{
	for (const auto& attribute_name : tag.get_attributes_names())
	{
		builder << L" ";
		builder << transform_by(attribute_name, format_attribute_name);
		builder << L"=";
		builder << attribute_restraint_mark;
		builder << transform_by(tag.get_attribute(attribute_name).value_or(L""), format_attribute_value);
		builder << attribute_restraint_mark;
	}
}

Pirus::text Pirus::TagFormatter::transform_by(std::wstring_view tag_name, Pirus::FORMAT_TEXT format_type)
{
	Pirus::text txt(tag_name);
	switch (format_type)
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
