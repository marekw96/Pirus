#include "gtest/gtest.h"

#include "Pirus/Tag.h"
#include "Pirus/TagFormatter.h"

TEST(one_single_tag, tag_formatter)
{
	Pirus::Tag tag(L"tag", Pirus::ALLOW_CHILDREN::NO);
	Pirus::TagFormatter formatter(tag);

	auto output = formatter.to_text();
	auto correct = L"<tag />";
	ASSERT_EQ(output, correct);
}

TEST(one_single_tag_with_atributes, tag_formatter)
{
	Pirus::Tag tag(L"tag", Pirus::ALLOW_CHILDREN::NO);
	Pirus::TagFormatter formatter(tag);
	tag.set_attribute(L"href", L"index");
	tag.set_attribute(L"class", L"container");

	auto output = formatter.to_text();
	auto attributes_order = tag.get_attributes_names();
	Pirus::text correct;
	if (attributes_order[0] == L"href")
		correct = L"<tag href=\"index\" class=\"container\" />";
	else
		correct = L"<tag class=\"container\" href=\"index\" />";

	ASSERT_EQ(output, correct);
}

TEST(one_container_tag, tag_formatter)
{
	Pirus::Tag tag(L"tag", Pirus::ALLOW_CHILDREN::YES);
	Pirus::TagFormatter formatter(tag);

	auto output = formatter.to_text();
	auto correct = L"<tag></tag>";
	ASSERT_EQ(output, correct);
}

TEST(one_container_tag_with_atributes, tag_formatter)
{
	Pirus::Tag tag(L"tag", Pirus::ALLOW_CHILDREN::YES);
	Pirus::TagFormatter formatter(tag);
	tag.set_attribute(L"href", L"index");
	tag.set_attribute(L"class", L"container");

	auto output = formatter.to_text();
	auto attributes_order = tag.get_attributes_names();
	Pirus::text correct;
	if (attributes_order[0] == L"href")
		correct = L"<tag href=\"index\" class=\"container\"></tag>";
	else
		correct = L"<tag class=\"container\" href=\"index\"></tag>";

	ASSERT_EQ(output, correct);
}

TEST(container_tag_with_single_tag_child, tag_formatter)
{
	Pirus::Tag tag_parent(L"parent", Pirus::ALLOW_CHILDREN::YES);
	Pirus::Tag tag_child(L"child", Pirus::ALLOW_CHILDREN::NO);
	tag_parent.add_child(tag_child);
	Pirus::TagFormatter formatter(tag_parent);
	auto output = formatter.to_text();
	auto correct = L"<parent>\n       <child />\n</parent>";

	ASSERT_EQ(output, correct);
}