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
					this->m_status = FRAGMENT_TYPE::TEXT;
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
				else if (sign == '/')
				{
					this->make_fragment();
					this->m_stream << sign;
					this->m_status = FRAGMENT_TYPE::TAG;
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
	if(this->m_stream.str().size() == 0)
		return;

	Pirus::Fragment f;
	f.type = this->m_status;
	f.value = this->m_stream.str();
	this->m_stream.str("");

	if(f.type != FRAGMENT_TYPE::TEXT)
	{
		if(f.value[0] == '/')
		{
			f.type = FRAGMENT_TYPE::CLOSE_TAG;
			
			if(f.value.size() > 1)
			{
				f.value.erase(0,1);
				this->m_level -= 2;
			}
			else
			{
				--this->m_level;
			}
		}
		if (f.type == FRAGMENT_TYPE::ATTRIBUTE_VALUE)
		{
			this->m_level -= 1;
			if(f.value[0] == '"')
				f.value = f.value.substr(1);
		}
		else if (f.type == FRAGMENT_TYPE::ATTRIBUTE_NAME)
		{
			this->m_level -= 1;
		}
	}

	f.level = this->m_level;
	++this->m_level;

	this->m_fragments.emplace_back(f);
}
