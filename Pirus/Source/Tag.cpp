#include "../Header/Tag.h"
#include <algorithm>

namespace Pirus
{
	Tag::Tag(const std::string & name, bool contains_content) : name(name), allow_content(contains_content)
	{
		this->prepare_name();
	}

	bool Tag::contains_content() const
	{
		return this->allow_content ? true : false;
	}

	void Tag::prepare_name()
	{
	//tolower
		std::transform(this->name.begin(), this->name.end(), this->name.begin(), ::tolower);
		
	//trim
		size_t first = this->name.find_first_not_of(' ');
		size_t last = this->name.find_last_not_of(' ');
		this->name = this->name.substr(first, (last - first + 1));
	}

	const std::string& Tag::get_name() const
	{
		return this->name;
	}
}