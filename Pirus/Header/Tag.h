#pragma once
#include <string>

namespace Pirus
{
	class Tag
	{
	public:
		Tag(const std::string& name,bool contains_content);
		const std::string& get_name() const;
		bool contains_content() const;
	
	private:
		uint8_t allow_content:1;
		std::string name;

		void prepare_name();
	};
}