#include "catch.hpp"

#include "Fragment.h"

TEST_CASE("Fragment", "[fragment]")
{
	SECTION("== operator")
	{
		Pirus::Fragment a;
		a.level = 0;
		a.type = Pirus::FRAGMENT_TYPE::ATTRIBUTE_NAME;
		a.value = "test";

		Pirus::Fragment b;
		b.level = 0;
		b.type = Pirus::FRAGMENT_TYPE::ATTRIBUTE_NAME;
		b.value = "test";

		REQUIRE(a == b);
		b.level = 1;
		REQUIRE_FALSE(a == b);

	}
}