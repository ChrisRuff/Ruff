#pragma once
#include <string>
#include <string_view>
#include <vector>

namespace ruff::nlp
{
class Tokenizer
{
public:
	virtual ~Tokenizer() = default;

public:
	virtual std::vector<std::string> ParseLine(const std::string_view) = 0;
};

template<typename T>
concept IsTokenizer = std::is_base_of<Tokenizer, T>::value;
}// namespace ruff::nlp
