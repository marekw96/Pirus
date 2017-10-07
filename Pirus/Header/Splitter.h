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

		void make_fragment();
	};
}