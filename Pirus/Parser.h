#pragma once

namespace Pirus {
	class Parser {
	public:
		Parser(const Parser&) = delete;
		Parser(Parser&&) = delete;
		Parser& operator=(const Parser&) = delete;
		Parser& operator=(Parser&&) = delete;
		virtual ~Parser() = default;

	private:

	};
}