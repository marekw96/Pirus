#pragma once
#include <string>

namespace Pirus
{
	class Tag
	{
	public:
		explicit Tag(const std::string& name, bool contains_content);
		Tag(const Tag&) = default;
		Tag(Tag&&) = default;
		~Tag() = default;
		Tag& operator=(const Tag&) = default;
		Tag& operator=(Tag&&) = default;
		const std::string& get_name() const;
		bool contains_content() const;
	
	private:
		uint8_t m_allow_content:1;
		std::string m_name;
		
		void prepare_name();
	};
}