#include "Parser.h"

#include "Splitter.h"

#include <iostream>

std::vector<Pirus::Tag> Pirus::parse(text content)
{
	Pirus::Splitter splitter;
	splitter.set_content(content);
	splitter.parse_to_fragments();
	splitter.parse_to_tags();
	return std::move(splitter.get_tags());
}
