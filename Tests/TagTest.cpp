#include "gtest\gtest.h"

#include "..\Pirus\Tag.h"

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