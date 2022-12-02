#include "doctest/doctest.h"

#include <iostream>
#include <string>

#include <ruff/nlp/simple_tokenizer.hpp>
TEST_SUITE("Tokenizer")
{
	TEST_CASE("Simple Tokenizer")
	{
		using namespace ruff::nlp;
		SimpleTokenizer tokenizer;

		std::string msg = "Hello World, This is tokenized";
		auto tokens = tokenizer.ParseLine(msg);

		CHECK(tokens[0] == "Hello");
		CHECK(tokens[1] == "World");
		CHECK(tokens[2] == ",");
		CHECK(tokens[3] == "This");
		CHECK(tokens[4] == "is");
		CHECK(tokens[5] == "tokenized");
	};
};
