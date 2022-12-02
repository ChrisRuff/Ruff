#pragma once
#include <ruff/nlp/tokenizer.hpp>

namespace ruff::nlp
{
class SimpleTokenizer : public Tokenizer
{
public:
	std::vector<std::string> ParseLine(const std::string_view txt) override;
};
}// namespace ruff::nlp