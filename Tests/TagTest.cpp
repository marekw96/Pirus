#include <algorithm>

#include "gtest\gtest.h"

#include "..\Pirus\Tag.h"
#include "Exceptions.h"

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

TEST(get_dont_allow_children, constructor)
{
	Pirus::Tag tag(L"test", Pirus::ALLOW_CHILDREN::NO);

	ASSERT_FALSE(tag.are_children_allowed());
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


TEST(exception_not_allowed_while_adding_child, children)
{
	Pirus::Tag tag(L"test", Pirus::ALLOW_CHILDREN::NO);

	ASSERT_THROW(tag.add_child(Pirus::Tag{L"child", Pirus::ALLOW_CHILDREN::YES}), Pirus::ChildrenNotAllowed);
}

TEST(add_child, children)
{
	Pirus::Tag tag(L"test", Pirus::ALLOW_CHILDREN::YES);

	ASSERT_NO_THROW(tag.add_child(Pirus::Tag (L"child", Pirus::ALLOW_CHILDREN::YES)));
	ASSERT_NO_THROW(tag.add_child(Pirus::Tag (L"child2", Pirus::ALLOW_CHILDREN::YES)));

	const auto& children = tag.get_children();

	ASSERT_EQ(children.size(), 2);
	ASSERT_EQ(children[0].get_name(), L"child");
	ASSERT_EQ(children[1].get_name(), L"child2");
}

TEST(remove_child, children)
{
	Pirus::Tag tag(L"test", Pirus::ALLOW_CHILDREN::YES);
	tag.add_child(Pirus::Tag(L"child", Pirus::ALLOW_CHILDREN::YES));
	tag.add_child(Pirus::Tag(L"child2", Pirus::ALLOW_CHILDREN::YES));
	tag.add_child(Pirus::Tag(L"child3", Pirus::ALLOW_CHILDREN::YES));

	ASSERT_EQ(tag.get_children().size(), 3);

	tag.remove_children_if([](const auto& child){ return child.get_name() == L"child2";});

	const auto& children = tag.get_children();
	ASSERT_EQ(children.size(), 2);
	ASSERT_EQ(children[0].get_name(), L"child");
	ASSERT_EQ(children[1].get_name(), L"child3");
}