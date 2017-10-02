#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "Tag.h"

TEST_CASE("Tag", "[node]")
{
	SECTION("c-tor")
	{
		Pirus::Tag test_node("img",0);

		REQUIRE( test_node.get_name() == "img");
		REQUIRE( test_node.contains_content() == false);
	}

	SECTION("c-tor text transform")
	{
		Pirus::Tag t(" iMg ",0);
		
		REQUIRE(t.get_name() == "img");
	}


}

