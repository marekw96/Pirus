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

TEST(container_tag_with_multiple_children, tag_formatter)
{
	Pirus::Tag tag_parent(L"parent", Pirus::ALLOW_CHILDREN::YES);
	Pirus::Tag container_child(L"container", Pirus::ALLOW_CHILDREN::YES);
	Pirus::Tag single_child(L"child", Pirus::ALLOW_CHILDREN::NO);
	Pirus::Tag single_child2(L"child2", Pirus::ALLOW_CHILDREN::NO);
	container_child.add_child(single_child);
	tag_parent.add_child(container_child);
	tag_parent.add_child(single_child2);

	Pirus::TagFormatter formatter(tag_parent);

	auto correct = L"<parent>\n       <container>\n              <child />\n       </container>\n       <child2 />\n</parent>";
	auto output = formatter.to_text();

	ASSERT_EQ(output, correct);
}

TEST(container_format_name_lowercase, tag_formatter)
{
	Pirus::Tag tag(L"TEST", Pirus::ALLOW_CHILDREN::YES);

	Pirus::TagFormatter formatter(tag);

	auto output = formatter.to_text();
	auto correct = L"<test></test>";

	ASSERT_EQ(output, correct);
}

TEST(container_format_name_capital, tag_formatter)
{
	Pirus::Tag tag(L"test", Pirus::ALLOW_CHILDREN::YES);

	Pirus::TagFormatter formatter(tag);
	formatter.set_format_tag_name(Pirus::FORMAT_TEXT::CAPITAL);

	auto output = formatter.to_text();
	auto correct = L"<TEST></TEST>";

	ASSERT_EQ(output, correct);
}

TEST(container_format_name_first_letter_capital, tag_formatter)
{
	Pirus::Tag tag(L"TEST", Pirus::ALLOW_CHILDREN::YES);

	Pirus::TagFormatter formatter(tag);
	formatter.set_format_tag_name(Pirus::FORMAT_TEXT::FIRST_LETTER_CAPITAL);

	auto output = formatter.to_text();
	auto correct = L"<Test></Test>";

	ASSERT_EQ(output, correct);
}

TEST(container_format_name_skip, tag_formatter)
{
	Pirus::Tag tag(L"TesT", Pirus::ALLOW_CHILDREN::YES);

	Pirus::TagFormatter formatter(tag);
	formatter.set_format_tag_name(Pirus::FORMAT_TEXT::SKIP);

	auto output = formatter.to_text();
	auto correct = L"<TesT></TesT>";

	ASSERT_EQ(output, correct);
}

TEST(container_tag_with_multiple_container_children, tag_formatter)
{
	Pirus::Tag tag_parent(L"parent", Pirus::ALLOW_CHILDREN::YES);
	Pirus::Tag container_child(L"container", Pirus::ALLOW_CHILDREN::YES);
	Pirus::Tag single_child(L"child", Pirus::ALLOW_CHILDREN::YES);
	Pirus::Tag single_child2(L"child2", Pirus::ALLOW_CHILDREN::YES);
	container_child.add_child(single_child);
	tag_parent.add_child(container_child);
	tag_parent.add_child(single_child2);

	Pirus::TagFormatter formatter(tag_parent);

	auto correct = L"<parent>\n       <container>\n              <child></child>\n       </container>\n       <child2></child2>\n</parent>";
	auto output = formatter.to_text();

	ASSERT_EQ(output, correct);
}