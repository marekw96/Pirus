#include "gtest/gtest.h"

#include "Tag.h"
#include "TagFormatter.h"

TEST(one_single_tag, tag_formatter)
{
	Pirus::Tag tag(L"tag", Pirus::ALLOW_CHILDREN::NO);
	Pirus::TagFormatter formatter(tag);

	auto output = formatter.to_text();
	auto correct = L"<tag />";
	ASSERT_EQ(output, correct);
}