#include "catch.hpp"

#include "Exceptions.h"
#include "TagBuilder.h"

TEST_CASE("tagbuilder", "[tag builder]")
{
	Pirus::TagBuilder builder;

	SECTION("has name == false")
	{
		builder.clear();
		REQUIRE(builder.has_name() == false);
	}

	SECTION("has name == true")
	{
		builder.clear().set_name("test_tag");
		REQUIRE(builder.has_name() == true);
	}

	SECTION("build without name")
	{
		builder.clear();
		REQUIRE_THROWS_AS(builder.build(), Pirus::NameNotSet);
	}
	
	SECTION("name set & allow childrn")
	{
		Pirus::text name = "test_tag";
		builder.clear().set_name(name).allow_children(true);
		auto tag = builder.build();
		REQUIRE(tag.get_name() == "test_tag");
		REQUIRE(tag.children_allowed() == true);
	}

	SECTION("name set & not allow children")
	{
		auto tag = builder.clear().set_name("test_tag").allow_children(Pirus::ALLOW_CHILDREN::NO).build();
		REQUIRE(tag.get_name() == "test_tag");
		REQUIRE(tag.children_allowed() == false);
	}

	SECTION("add attribute")
	{
		auto tag = builder.clear().set_name("test_tag").set_attribute("style", "color: red;").build();
		REQUIRE(tag.get_attributes_names() == std::vector<Pirus::text>{"style"});
		REQUIRE(tag.get_attribute("style") == "color: red;");
	}
}