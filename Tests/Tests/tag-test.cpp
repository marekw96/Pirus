#include "catch.hpp"

#include "Tag.h"

TEST_CASE("Tag", "[tag]")
{
	SECTION("c-tor")
	{
		Pirus::Tag test_tag("img",Pirus::ALLOW_CHILDREN::NO);

		REQUIRE( test_tag.get_name() == "img");
		REQUIRE( test_tag.children_allowed() == false);
		REQUIRE( test_tag.count_children() == 0);
	}

	SECTION("c-tor text transform")
	{
		Pirus::Tag t(" iMg ", Pirus::ALLOW_CHILDREN::NO);
		
		REQUIRE(t.get_name() == "img");
	}

	SECTION("add attributes")
	{
		Pirus::Tag t("test", Pirus::ALLOW_CHILDREN::NO);
		t.add_attribute("style","color","#ffffff");

		REQUIRE(t.get_attribute("style","color") == "#ffffff");
	}

	SECTION("getting vector of attributes")
	{
		Pirus::Tag t("test", Pirus::ALLOW_CHILDREN::NO);
		t.add_attribute("style", "color", "#ffffff");
		t.add_attribute("style", "text-align", "center");

		std::vector<std::pair<std::string,std::string>> vector{{"color", "#ffffff"},{"text-align","center"}};

		REQUIRE(t.get_attributes("style") == vector);
	}

	SECTION("attributes - throw exception if not found")
	{
		Pirus::Tag t("test", Pirus::ALLOW_CHILDREN::NO);
		t.add_attribute("style", "color", "#ffffff");

		REQUIRE_THROWS(t.get_attribute("style","not_found"));
	}

	SECTION("getting vector of attributes")
	{
		Pirus::Tag t("test", Pirus::ALLOW_CHILDREN::NO);
		t.add_attribute("style", "color", "#ffffff");
		t.add_attribute("ng-repeat", "", "center");

		auto atr = t.get_attributes_names();

		REQUIRE(atr.size() == 2);
		REQUIRE(std::any_of(atr.begin(), atr.end(), [] (const auto& el) {return el == "style"; }));
		REQUIRE(std::any_of(atr.begin(), atr.end(), [] (const auto& el) { return el == "ng-repeat"; }));
	}

	SECTION("setting new value to already set one")
	{
		Pirus::Tag t("test", Pirus::ALLOW_CHILDREN::NO);
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
		Pirus::Tag t("test", Pirus::ALLOW_CHILDREN::NO);
		t.add_attribute("style", "color", "#ffffff");

		REQUIRE(t.get_attribute("style", "color") == "#ffffff");
		REQUIRE(t.get_attributes("style").size() == 1u);
		REQUIRE(t.remove_attribute("style", "color") == true);//remove
		REQUIRE_THROWS(t.get_attribute("style","color"));
		REQUIRE(t.get_attributes("style").size() == 0u);
	}

	SECTION("operator << ostream& - only name, no content")
	{
		Pirus::Tag t("test", Pirus::ALLOW_CHILDREN::NO);

		std::stringstream stream;
		stream << t;

		REQUIRE(stream.str() == "<test />");
	}

	SECTION("operator << ostream& - only name, empty content")
	{
		Pirus::Tag t("test", Pirus::ALLOW_CHILDREN::YES);

		std::stringstream stream;
		stream << t;

		REQUIRE(stream.str() == "<test></test>");
	}

	SECTION("operator << ostream& - only name, empty content, one attribute")
	{
		Pirus::Tag t("test", Pirus::ALLOW_CHILDREN::YES);
		t.add_attribute("style", "color", "#ffffff");

		std::stringstream stream;
		stream << t;

		REQUIRE(stream.str() == "<test style=\"color: #ffffff;\"></test>");
	}

	SECTION("operator << ostream& - only name, empty content, one attribute, multiple values")
	{
		Pirus::Tag t("test", Pirus::ALLOW_CHILDREN::YES);
		t.add_attribute("style", "color", "#ffffff");
		t.add_attribute("style", "text-align", "center");
		t.add_attribute("style", "border", "none");

		std::stringstream stream;
		stream << t;

		REQUIRE(stream.str() == "<test style=\"color: #ffffff;text-align: center;border: none;\"></test>");
	}

	SECTION("operator << - name, empty content, two attributes")
	{
		Pirus::Tag t("test", Pirus::ALLOW_CHILDREN::YES);
		t.add_attribute("style", "color", "#ffffff");
		t.add_attribute("on-click", "", "alert('aa')");

		std::stringstream stream;
		stream << t;

		std::string str = stream.str();
		
		REQUIRE(str.size() == 60);
		REQUIRE(str.find("<test ") == 0);
		REQUIRE(str.find("></test>") == 60 - 8);
		REQUIRE(str.find(" style=\"color: #ffffff;\"") != std::string::npos );
		REQUIRE(str.find(" on-click=\"alert('aa')\"") != std::string::npos );
	}

	SECTION("attribute_exists")
	{
		Pirus::Tag t("test", Pirus::ALLOW_CHILDREN::YES);
		t.add_attribute("style", "color", "#ffffff");

		REQUIRE(t.attribute_exists("style", "color") == true);
		REQUIRE(t.attribute_exists("style", "text-align") == false);
	}

	SECTION("add child")
	{
		Pirus::Tag t("test", Pirus::ALLOW_CHILDREN::YES);

		REQUIRE(t.count_children() == 0);

		REQUIRE_NOTHROW(t.add_child(Pirus::Tag("child", Pirus::ALLOW_CHILDREN::NO)));
		REQUIRE(t.count_children() == 1);

		Pirus::Tag child("child2", Pirus::ALLOW_CHILDREN::YES);
		REQUIRE_NOTHROW(t.add_child(child));

		REQUIRE(t.count_children() == 2);
	}

	SECTION("clear children")
	{
		Pirus::Tag t("test", Pirus::ALLOW_CHILDREN::YES);
		t.add_child(Pirus::Tag("child", Pirus::ALLOW_CHILDREN::NO));
		t.add_child(Pirus::Tag("child", Pirus::ALLOW_CHILDREN::NO));
		t.add_child(Pirus::Tag("child", Pirus::ALLOW_CHILDREN::NO));

		REQUIRE(t.count_children() == 3);
		t.clear_children();
		REQUIRE(t.count_children() == 0);
		REQUIRE(t.get_children().size() == 0);
		REQUIRE(t.get_text().size() == 0);
	}

	SECTION("operator << & to_text - children")
	{
		Pirus::Tag t("test", Pirus::ALLOW_CHILDREN::YES);
		t.add_child(Pirus::Tag("child", Pirus::ALLOW_CHILDREN::NO));

		std::stringstream stream;
		stream << t;

		REQUIRE( stream.str() == "<test>\n\t<child />\n</test>");
		REQUIRE( t.to_text() == "<test>\n\t<child />\n</test>");
	}

	SECTION("operator << & to_text - children & attributes")
	{
		Pirus::Tag a("a", Pirus::ALLOW_CHILDREN::YES);
		Pirus::Tag img("img", Pirus::ALLOW_CHILDREN::NO);

		img.add_attribute("src","","test.jpg");
		a.add_attribute("style","color","#ffffff");

		a.add_child(Pirus::Tag(img));

		std::stringstream stream;
		stream << a;

		REQUIRE( a.to_text() == "<a style=\"color: #ffffff;\">\n\t<img src=\"test.jpg\" />\n</a>");
		REQUIRE(stream.str() == "<a style=\"color: #ffffff;\">\n\t<img src=\"test.jpg\" />\n</a>");
	}

	SECTION("operator << & to_text - few children")
	{
		Pirus::Tag a("a", 1);
		Pirus::Tag b("b", 1);
		b.add_child(Pirus::Tag("c",0));
		a.add_child("Text");
		Pirus::Tag d("d",1);
		d.add_child(a);
		d.add_child(b);

		std::stringstream stream;
		stream << d;

		REQUIRE( d.to_text() == "<d>\n\t<a>\n\t\tText\n\t</a>\n\t<b>\n\t\t<c />\n\t</b>\n</d>");
		REQUIRE(stream.str() == "<d>\n\t<a>\n\t\tText\n\t</a>\n\t<b>\n\t\t<c />\n\t</b>\n</d>");
	}

	SECTION("to_text - empty tag")
	{
		Pirus::Tag a("a", 1);
		a.add_child(Pirus::Tag("b", 1));

		REQUIRE(a.to_text() == "<a>\n\t<b></b>\n</a>");
	}

	SECTION("text child -add, not allowed by tag type")
	{
		Pirus::Tag test("text", Pirus::ALLOW_CHILDREN::NO);
		REQUIRE_THROWS(test.add_child("text"));
	}

	SECTION("text child -add if different child")
	{
		Pirus::Tag test("text", Pirus::ALLOW_CHILDREN::NO);
		Pirus::Tag f("test", Pirus::ALLOW_CHILDREN::YES);
		REQUIRE(f.get_text() == "");
		REQUIRE_NOTHROW(f.add_child(test));
		REQUIRE_THROWS(f.add_child("TEST"));
	}

	SECTION("text child - print")
	{
		Pirus::Tag f("test", Pirus::ALLOW_CHILDREN::YES);
		REQUIRE_NOTHROW(f.add_child("txt"));
		REQUIRE(f.get_text() == "txt");
		REQUIRE(f.count_children() == 1);

		std::stringstream stream;
		stream << f;
		REQUIRE(stream.str() == "<test>\n\ttxt\n</test>");
	}

	SECTION("add child when text is added")
	{
		Pirus::Tag f("test", Pirus::ALLOW_CHILDREN::YES);
		REQUIRE_NOTHROW(f.add_child("txt"));
		REQUIRE(f.get_text() == "txt");
		REQUIRE_THROWS(f.add_child(Pirus::Tag("text", Pirus::ALLOW_CHILDREN::NO)));
	}

	SECTION("get children")
	{
		Pirus::Tag t("t", Pirus::ALLOW_CHILDREN::YES);
		t.add_child(Pirus::Tag("a1", Pirus::ALLOW_CHILDREN::NO));
		t.add_child(Pirus::Tag("a2", Pirus::ALLOW_CHILDREN::NO));

		REQUIRE(t.count_children() == 2);
		REQUIRE(t.get_children()[0].get_name() == "a1");
		REQUIRE(t.get_children()[1].get_name() == "a2");
	}

	SECTION("get children - const")
	{
		Pirus::Tag t("t", Pirus::ALLOW_CHILDREN::YES);
		t.add_child(Pirus::Tag("a1", Pirus::ALLOW_CHILDREN::NO));
		t.add_child(Pirus::Tag("a2", Pirus::ALLOW_CHILDREN::NO));

		const auto& const_t = t;

		REQUIRE(const_t.count_children() == 2);
		REQUIRE(const_t.get_children()[0].get_name() == "a1");
		REQUIRE(const_t.get_children()[1].get_name() == "a2");
	}

	SECTION("CHILD_TYPE :: NONE")
	{
		Pirus::Tag t("test", Pirus::ALLOW_CHILDREN::YES);
		REQUIRE(t.get_type_of_children() == Pirus::CHILD_TYPE::NONE);
		REQUIRE_FALSE(t.get_type_of_children() == Pirus::CHILD_TYPE::NOT_ALLOWED);
		REQUIRE_FALSE(t.get_type_of_children() == Pirus::CHILD_TYPE::TAG);
		REQUIRE_FALSE(t.get_type_of_children() == Pirus::CHILD_TYPE::TEXT);
	}

	SECTION("CHILD_TYPE :: NOT_ALLOWED")
	{
		Pirus::Tag t("test", Pirus::ALLOW_CHILDREN::NO);
		REQUIRE_FALSE(t.get_type_of_children() == Pirus::CHILD_TYPE::NONE);
		REQUIRE(t.get_type_of_children() == Pirus::CHILD_TYPE::NOT_ALLOWED);
		REQUIRE_FALSE(t.get_type_of_children() == Pirus::CHILD_TYPE::TAG);
		REQUIRE_FALSE(t.get_type_of_children() == Pirus::CHILD_TYPE::TEXT);
	}

	SECTION("CHILD_TYPE :: TAG")
	{
		Pirus::Tag t("test", Pirus::ALLOW_CHILDREN::YES);
		t.add_child(Pirus::Tag("child", Pirus::ALLOW_CHILDREN::NO));
		REQUIRE_FALSE(t.get_type_of_children() == Pirus::CHILD_TYPE::NONE);
		REQUIRE_FALSE(t.get_type_of_children() == Pirus::CHILD_TYPE::NOT_ALLOWED);
		REQUIRE(t.get_type_of_children() == Pirus::CHILD_TYPE::TAG);
		REQUIRE_FALSE(t.get_type_of_children() == Pirus::CHILD_TYPE::TEXT);
	}

	SECTION("CHILD_TYPE :: TEXT")
	{
		Pirus::Tag t("test", Pirus::ALLOW_CHILDREN::YES);
		t.add_child("txt");
		REQUIRE_FALSE(t.get_type_of_children() == Pirus::CHILD_TYPE::NONE);
		REQUIRE_FALSE(t.get_type_of_children() == Pirus::CHILD_TYPE::NOT_ALLOWED);
		REQUIRE_FALSE(t.get_type_of_children() == Pirus::CHILD_TYPE::TAG);
		REQUIRE(t.get_type_of_children() == Pirus::CHILD_TYPE::TEXT);
	}
}

