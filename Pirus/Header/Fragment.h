#pragma once
#include "Constants.h"

namespace Pirus
{
	enum class FRAGMENT_TYPE
	{
		TAG,
		OPEN_TAG,
		TAG_NAME,
		ATTRIBUTE_NAME,
		ATTRIBUTE_VALUE,
		TEXT,
		CLOSE_TAG,
		COMMENT,
		NONE,
		POSSIBLE_COMMENT
	};

	struct Fragment
	{
		text value;
		size_t level;
		FRAGMENT_TYPE type;

		bool operator==(const Fragment& f) const;
	};

	
}