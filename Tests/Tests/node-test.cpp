#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "Tag.h"

TEST_CASE("Tag", "[node]")
{
	SECTION("c-tor")
	{
		Pirus::Tag test_node("img",false);

		REQUIRE( test_node.get_name() == "img");
		REQUIRE( test_node.children_allowed() == false);
		REQUIRE( test_node.count_children() == 0);
	}

	SECTION("c-tor text transform")
	{
		Pirus::Tag t(" iMg ",false);
		
		REQUIRE(t.get_name() == "img");
	}

	SECTION("add attributes")
	{
		Pirus::Tag t("test",0);
		t.add_attribute("style","color","#ffffff");

		REQUIRE(t.get_attribute("style","color") == "#ffffff");
	}

	SECTION("getting vector of attributes")
	{
		Pirus::Tag t("test", 0);
		t.add_attribute("style", "color", "#ffffff");
		t.add_attribute("style", "text-align", "center");

		std::vector<std::pair<std::string,std::string>> vector{{"color", "#ffffff"},{"text-align","center"}};

		REQUIRE(t.get_attributes("style") == vector);
	}

	SECTION("attributes - throw exception if not found")
	{
		Pirus::Tag t("test", 0);
		t.add_attribute("style", "color", "#ffffff");

		REQUIRE_THROWS(t.get_attribute("style","not_found"));
	}

	SECTION("getting vector of attributes")
	{
		Pirus::Tag t("test", 0);
		t.add_attribute("style", "color", "#ffffff");
		t.add_attribute("ng-repeat", "", "center");

		std::vector<std::string> vector{"style","ng-repeat"};

		REQUIRE(t.get_attributes_names() == vector);
	}

	SECTION("setting new value to already set one")
	{
		Pirus::Tag t("test", 0);
		t.add_attribute("style", "color", "#ffffff");
		//begin
		std::vector<std::pair<std::string, std::string>> vector {{"color", "#ffffff"}};
		REQUIRE(t.get_attributes("style") == vector);

		//change color
		t.add_attribute("style", "color", "#000000");
		REQUIRE(t.get_attribute("style", "color") == "#000000");

		//after editing
		vector = {{"color", "#000000"}};
		REQUIRE(t.get_attributes("style") == vector);
	}

	SECTION("remove attribute")
	{
		Pirus::Tag t("test", 0);
		t.add_attribute("style", "color", "#ffffff");

		REQUIRE(t.get_attribute("style", "color") == "#ffffff");
		REQUIRE(t.get_attributes("style").size() == 1u);
		REQUIRE(t.remove_attribute("style", "color") == true);//remove
		REQUIRE_THROWS(t.get_attribute("style","color"));
		REQUIRE(t.get_attributes("style").size() == 0u);
	}

	SECTION("operator << ostream& - only name, no content")
	{
		Pirus::Tag t("test", 0);

		std::stringstream stream;
		stream << t;

		REQUIRE(stream.str() == "<test />");
	}

	SECTION("operator << ostream& - only name, empty content")
	{
		Pirus::Tag t("test", 1);

		std::stringstream stream;
		stream << t;

		REQUIRE(stream.str() == "<test></test>");
	}

	SECTION("operator << ostream& - only name, empty content, one attribute")
	{
		Pirus::Tag t("test", 1);
		t.add_attribute("style", "color", "#ffffff");

		std::stringstream stream;
		stream << t;

		REQUIRE(stream.str() == "<test style=\"color: #ffffff;\"></test>");
	}

	SECTION("operator << ostream& - only name, empty content, one attribute, multiple values")
	{
		Pirus::Tag t("test", 1);
		t.add_attribute("style", "color", "#ffffff");
		t.add_attribute("style", "text-align", "center");
		t.add_attribute("style", "border", "none");

		std::stringstream stream;
		stream << t;

		REQUIRE(stream.str() == "<test style=\"color: #ffffff;text-align: center;border: none;\"></test>");
	}

	SECTION("operator << - name, empty content, two attributes")
	{
		Pirus::Tag t("test", 1);
		t.add_attribute("style", "color", "#ffffff");
		t.add_attribute("on-click", "", "alert('aa')");

		std::stringstream stream;
		stream << t;

		REQUIRE(stream.str() == "<test style=\"color: #ffffff;\" on-click=\"alert('aa')\"></test>");
	}

	SECTION("attribute_exists")
	{
		Pirus::Tag t("test", 1);
		t.add_attribute("style", "color", "#ffffff");

		REQUIRE(t.attribute_exists("style", "color") == true);
		REQUIRE(t.attribute_exists("style", "text-align") == false);
	}

	SECTION("add child")
	{
		Pirus::Tag t("test", 1);

		REQUIRE(t.count_children() == 0);

		t.add_child(Pirus::Tag("child",0));
		REQUIRE(t.count_children() == 1);

		Pirus::Tag child("child2", 1);
		t.add_child(child);

		REQUIRE(t.count_children() == 2);
	}

	SECTION("clear children")
	{
		Pirus::Tag t("test", 1);
		t.add_child(Pirus::Tag("child", 0));
		t.add_child(Pirus::Tag("child", 0));
		t.add_child(Pirus::Tag("child", 0));

		REQUIRE(t.count_children() == 3);
	}

	SECTION("operator << - children")
	{
		Pirus::Tag t("test", 1);
		t.add_child(Pirus::Tag("child", 0));

		std::stringstream stream;
		stream << t;

		REQUIRE( stream.str() == "<test><child /></test>");
	}

	SECTION("operator << - children & attributes")
	{
		Pirus::Tag a("a", 1);
		Pirus::Tag img("img", 0);

		img.add_attribute("src","","test.jpg");
		a.add_attribute("style","color","#ffffff");

		a.add_child(Pirus::Tag(img));

		std::stringstream stream;
		stream << a;

		REQUIRE(stream.str() == "<a style=\"color: #ffffff;\"><img src=\"test.jpg\" /></a>");
	}

	SECTION("text operations")
	{
		Pirus::Tag test("text",0);
		REQUIRE_THROWS(test.set_text("text"));

		Pirus::Tag f("test",1);
		REQUIRE(f.get_text() == "");

		f.add_child(test);
		REQUIRE(f.count_children() == 1);

		REQUIRE_NOTHROW(f.set_text("txt"));
		REQUIRE(f.get_text() == "txt");
		REQUIRE(f.count_children() == 0);

		std::stringstream stream;
		stream << f;
		REQUIRE(stream.str() == "<test>txt</test>");

		f.add_child(test);
		REQUIRE(f.count_children() == 1);
		REQUIRE(f.get_text() == "");
	}

	SECTION("get children")
	{
		Pirus::Tag t("t",1);
		t.add_child(Pirus::Tag("a1", false));
		t.add_child(Pirus::Tag("a2", false));

		REQUIRE(t.count_children() == 2);
		REQUIRE(t.get_children()[0].get_name() == "a1");
		REQUIRE(t.get_children()[1].get_name() == "a2");
	}

	SECTION("get children - const")
	{
		Pirus::Tag t("t", 1);
		t.add_child(Pirus::Tag("a1", false));
		t.add_child(Pirus::Tag("a2", false));

		const auto& const_t = t;

		REQUIRE(const_t.count_children() == 2);
		REQUIRE(const_t.get_children()[0].get_name() == "a1");
		REQUIRE(const_t.get_children()[1].get_name() == "a2");
	}
}

