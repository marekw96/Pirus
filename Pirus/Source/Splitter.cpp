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
				this->parse_text(sign);
			break;
			case FRAGMENT_TYPE::TAG:
				this->parse_tag(sign);
			break;
			case FRAGMENT_TYPE::ATTRIBUTE_NAME:
				this->parse_attribute_name(sign);
			break;
			case FRAGMENT_TYPE::ATTRIBUTE_VALUE:
				this->parse_attribute_value(sign);
			break;
		}
	}

	return this->m_fragments;
}

std::vector<Pirus::Fragment>& Pirus::Splitter::get_fragments()
{
	return this->m_fragments;
}

Pirus::Fragment Pirus::Splitter::make_fragment()
{
	if(this->m_stream.str().size() == 0)
		return Pirus::Fragment();

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
				f.value.erase(0, 1);
		}
		else if (f.type == FRAGMENT_TYPE::ATTRIBUTE_NAME)
		{
			this->m_level -= 1;
		}
	}

	f.level = this->m_level++;

	return f;
}

template<typename T>
void Pirus::Splitter::parse_text(T sign)
{
	if (sign == '<')
	{
		if (this->m_stream.str().size() > 0)
			this->m_fragments.emplace_back(this->make_fragment());
		this->m_status = FRAGMENT_TYPE::TAG;
	}
	else
	{
		this->m_stream << sign;
	}
}

template<typename T>
void Pirus::Splitter::parse_tag(T sign)
{
	if (sign == '>')
	{
		if (this->m_stream.str().size() > 0)
			this->m_fragments.emplace_back(this->make_fragment());
		this->m_status = FRAGMENT_TYPE::TEXT;
	}
	else if (sign == ' ')
	{
		if (this->m_stream.str().size() > 0)
			this->m_fragments.emplace_back(this->make_fragment());
		this->m_status = FRAGMENT_TYPE::ATTRIBUTE_NAME;
	}
	else
	{
		this->m_stream << sign;
	}
}

template<typename T>
void Pirus::Splitter::parse_attribute_name(T sign)
{
	if (sign == '=')
	{
		if (this->m_stream.str().size() > 0)
			this->m_fragments.emplace_back(this->make_fragment());

		this->m_status = FRAGMENT_TYPE::ATTRIBUTE_VALUE;
		this->m_escape = true;
	}
	else if (sign == '>')
	{
		if (this->m_stream.str().size() > 0)
			this->m_fragments.emplace_back(this->make_fragment());

		this->m_status = FRAGMENT_TYPE::TEXT;
	}
	else if (sign == '/')
	{
		if (this->m_stream.str().size() > 0)
			this->m_fragments.emplace_back(this->make_fragment());

		this->m_stream << sign;
		this->m_status = FRAGMENT_TYPE::TAG;
	}
	else
	{
		this->m_stream << sign;
	}
}

template<typename T>
void Pirus::Splitter::parse_attribute_value(T sign)
{
	if (sign == '"' && this->m_escape == false)
	{
		if(this->m_stream.str().size() > 0)
			this->m_fragments.emplace_back(this->make_fragment());
		this->m_status = FRAGMENT_TYPE::TAG;
	}
	else
	{
		this->m_stream << sign;
	}
	this->m_escape = false;
}
