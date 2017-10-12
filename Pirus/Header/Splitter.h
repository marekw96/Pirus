#pragma once
#include "Constants.h"
#include "Exceptions.h"
#include "Fragment.h"
#include <vector>
#include <sstream>

namespace Pirus
{
	class Splitter
	{
	public:
		Splitter();
		std::vector<Pirus::Fragment>& operator()(const string& code);
		std::vector<Pirus::Fragment>& get_fragments();

	private:
		FRAGMENT_TYPE m_status;
		std::vector<Pirus::Fragment> m_fragments;
		size_t m_level;
		std::stringstream m_stream;
		bool m_escape;

		Pirus::Fragment make_fragment();

		template <typename T>
		void parse_text(T sign);
		template <typename T>
		void parse_tag(T sign);
		template <typename T>
		void parse_attribute_name(T sign);
		template <typename T>
		void parse_attribute_value(T sign);
	};
}