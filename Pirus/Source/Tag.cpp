#include "../Header/Tag.h"
#include <algorithm>

namespace Pirus
{
	Tag::Tag(const std::string & name, bool contains_content) : m_name(name), m_allow_content(contains_content)
	{
		this->prepare_name();
	}

	bool Tag::contains_content() const
	{
		return this->m_allow_content ? true : false;
	}

	void Tag::prepare_name()
	{
	//tolower
		std::transform(this->m_name.begin(), this->m_name.end(), this->m_name.begin(), ::tolower);
		
	//trim
		size_t first = this->m_name.find_first_not_of(' ');
		size_t last = this->m_name.find_last_not_of(' ');
		this->m_name = this->m_name.substr(first, (last - first + 1));
	}

	const std::string& Tag::get_name() const
	{
		return this->m_name;
	}
}