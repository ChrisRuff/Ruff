#include <ruff/nlp/simple_tokenizer.hpp>
#include <ctre.hpp>

namespace ruff::nlp
{
std::vector<std::string> SimpleTokenizer::ParseLine(const std::string_view line)
{
	std::vector<std::string> tokens;
	for(auto word : ctre::range<"\\w+|\\S+">(line))
	{
		tokens.emplace_back(word.get<0>());
	}
	return tokens;
}
}// namespace ruff::nlp