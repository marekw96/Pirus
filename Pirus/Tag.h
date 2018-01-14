#pragma once

#include "Definitions.h"
#include <optional>

namespace Pirus
{
	class Tag
	{
	public:
		Tag(const Pirus::text& name, Pirus::ALLOW_CHILDREN allow_children);
	
		const Pirus::text& get_name() const;

		std::optional<Pirus::text> get_attribute(const Pirus::text& name) const;
		void set_attribute(const Pirus::text& attribute_name, const Pirus::text& value);
	
		virtual ~Tag() = default;

	private:
		Pirus::text name;
		Pirus::ALLOW_CHILDREN allow_children;
		Pirus::attributes_map attributes;

	};
}