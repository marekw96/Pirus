#include "catch.hpp"

#include "Splitter.h"
#include "Fragment.h"
#include <vector>

TEST_CASE("Splitter", "[splitter]")
{
	SECTION("simple")
	{
		auto str = "<a>TEXT</a>";
		Pirus::Splitter s;
		s(str);

		auto& fragments = s.get_fragments();
		REQUIRE(fragments.size() == 3);
		
		Pirus::Fragment a1;
		a1.level = 0;
		a1.type = Pirus::FRAGMENT_TYPE::OPEN_TAG;
		a1.value = "a";

		Pirus::Fragment text;
		text.level = 1;
		text.type = Pirus::FRAGMENT_TYPE::TEXT;
		text.value = "TEXT";

		Pirus::Fragment a2;
		a1.level = 0;
		a1.type = Pirus::FRAGMENT_TYPE::CLOSE_TAG;
		a1.value = "a";

		std::vector<Pirus::Fragment> correct_fragments = {a1,text,a2};

		REQUIRE(fragments == correct_fragments);
	}
}