#include "Splitter.h"


Pirus::Splitter::Splitter() :
	m_status(FRAGMENT_TYPE::TEXT),
	m_fragments{},
	m_level{},
	m_stream{},
	m_escape{false}
{}

std::vector<Pirus::Fragment>& Pirus::Splitter::operator()(const string & code)
{
	for (auto sign : code)
	{
		switch (this->m_status)
		{
			case FRAGMENT_TYPE::TEXT:
				if(sign == '<')
				{
					this->make_fragment();
					this->m_status = FRAGMENT_TYPE::TAG;
				}
				else
				{
					this->m_stream << sign;
				}
			break;
			case FRAGMENT_TYPE::TAG:
				if(sign == '>')
				{
					this->make_fragment();
				}
				else if (sign == ' ')
				{
					this->make_fragment();
					this->m_status = FRAGMENT_TYPE::ATTRIBUTE_NAME;
				}
				else
				{
					this->m_stream << sign;
				}
			break;
			case FRAGMENT_TYPE::ATTRIBUTE_NAME:
				if (sign == '=')
				{
					this->make_fragment();
					this->m_status = FRAGMENT_TYPE::ATTRIBUTE_VALUE;
					this->m_escape = true;
				}
				else if (sign == '>')
				{
					this->make_fragment();
					this->m_status = FRAGMENT_TYPE::TEXT;
				}
				else
				{
					this->m_stream << sign;
				}
			break;
			case FRAGMENT_TYPE::ATTRIBUTE_VALUE:
				if (sign == '"' && this->m_escape == false)
				{
					this->make_fragment();
					this->m_status = FRAGMENT_TYPE::TAG;
				}
				else
				{
					this->m_stream << sign;
				}
				this->m_escape = false;
			break;
		}
	}

	return this->m_fragments;
}

std::vector<Pirus::Fragment>& Pirus::Splitter::get_fragments()
{
	return this->m_fragments;
}

void Pirus::Splitter::make_fragment()
{
	Pirus::Fragment f;
	f.level = this->m_level;
	f.type = this->m_status;
	f.value = this->m_stream.str();
	this->m_stream.clear();

	this->m_fragments.emplace_back(f);
}
