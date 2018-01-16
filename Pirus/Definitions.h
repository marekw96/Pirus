#pragma once

#include <string>
#include <unordered_map>

namespace Pirus
{
	using text = std::wstring;
	using attributes_map = std::unordered_map<Pirus::text, Pirus::text>;

	enum class ALLOW_CHILDREN {YES, NO};
	enum class FORMAT_TEXT{CAPITAL, LOWERCASE, FIRST_LETTER_CAPITAL, SKIP};

	namespace DefaultFormatterOptions
	{
		constexpr auto indention = L"       ";
		constexpr auto new_line = L"\n";
		constexpr auto open_tag = L"<";
		constexpr auto close_tag = L">";
		constexpr auto single_tag_close_mark = L"/";
		constexpr auto container_tag_close_mark = L"/";
		constexpr auto attribute_restraint_mark = L"\"";

		constexpr auto format_tag_name = Pirus::FORMAT_TEXT::LOWERCASE;
		constexpr auto format_attribute_name = Pirus::FORMAT_TEXT::LOWERCASE;
		constexpr auto format_attribute_value = Pirus::FORMAT_TEXT::SKIP;
	}
}