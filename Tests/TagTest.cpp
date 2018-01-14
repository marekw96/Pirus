#include <algorithm>

#include "gtest\gtest.h"

#include "..\Pirus\Tag.h"

TEST(get_name, constructor)
{
	Pirus::Tag tag(L"test", Pirus::ALLOW_CHILDREN::YES);

	ASSERT_EQ(tag.get_name(), L"test");
}

TEST(get_allow_children, constructor)
{
	Pirus::Tag tag(L"test", Pirus::ALLOW_CHILDREN::YES);
	
	ASSERT_TRUE(tag.are_children_allowed());
}

TEST(get_not_avaiable_attribute, attributes)
{
	Pirus::Tag tag(L"test",Pirus::ALLOW_CHILDREN::YES);

	ASSERT_FALSE(tag.get_attribute(L"href").has_value());
}

TEST(get_avaiable_attribute, attributes)
{
	Pirus::Tag tag(L"test", Pirus::ALLOW_CHILDREN::YES);
	tag.set_attribute(L"href", L"index.html");

	ASSERT_TRUE(tag.get_attribute(L"href").has_value());
	ASSERT_EQ(tag.get_attribute(L"href").value(), L"index.html");
}

TEST(reset_attribute, attributes)
{
	Pirus::Tag tag(L"test", Pirus::ALLOW_CHILDREN::YES);
	
	tag.set_attribute(L"href", L"index");
	ASSERT_EQ(tag.get_attribute(L"href").value(), L"index");

	tag.set_attribute(L"href", L"index2");
	ASSERT_EQ(tag.get_attribute(L"href").value(), L"index2");
}

TEST(remove_attribute, attributes)
{
	Pirus::Tag tag(L"test", Pirus::ALLOW_CHILDREN::YES);

	tag.set_attribute(L"href", L"index");
	ASSERT_EQ(tag.get_attribute(L"href").value(), L"index");

	tag.remove_attribute(L"href");
	ASSERT_FALSE(tag.get_attribute(L"href").has_value());
}

TEST(get_attributes_names, attributes)
{
	Pirus::Tag tag(L"test", Pirus::ALLOW_CHILDREN::YES);

	tag.set_attribute(L"href", L"index");
	tag.set_attribute(L"alt", L"");

	auto names = tag.get_attributes_names();

	ASSERT_EQ(names.size(), 2);
	std::sort(begin(names), end(names));

	std::vector<Pirus::text> correct = {L"alt", L"href"};
	ASSERT_EQ(names, correct);
}