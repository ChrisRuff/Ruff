#include "doctest/doctest.h"

#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>

#include <ruff/nlp/simple_tokenizer.hpp>
#include <ruff/nlp/gram_model.hpp>
#include <ruff/nlp/laplace_gram_model.hpp>

TEST_SUITE("Gram Models")
{
	TEST_CASE("Unigram")
	{
		ruff::nlp::GramModel<ruff::nlp::SimpleTokenizer, 1> model;

		std::string msg = "Hello World, This is tokenized.";
		model.ReadLine(msg);

		std::string sentence = model.GetStarter();
		std::string word;
		do {
			word = model.Choose(sentence);
			sentence += " " + word;
		} while(word != "</s>");
	};

	TEST_CASE("Bigram")
	{
		ruff::nlp::GramModel<ruff::nlp::SimpleTokenizer, 2> model;

		std::string msg = "Hello World, This is tokenized.";
		model.ReadLine(msg);

		std::string sentence = model.GetStarter();
		std::string word;
		do {
			word = model.Choose(sentence);
			sentence += " " + word;
		} while(word != "</s>");
		sentence = sentence.substr(4);                     // Remove <s>
		sentence = sentence.substr(0, sentence.size() - 5);// Remove </s>
		sentence.erase(sentence.begin() + 11);
		sentence.erase(sentence.end() - 2);
		CHECK(msg == sentence);// Small corpus == Same sentence
	}
	TEST_CASE("Trigram")
	{
		ruff::nlp::GramModel<ruff::nlp::SimpleTokenizer, 3> model;

		std::string msg = "Hello World, This is tokenized.";
		model.ReadLine(msg);

		std::string sentence = model.GetStarter();
		std::string word;
		do {
			word = model.Choose(sentence);
			sentence += " " + word;
		} while(word != "</s>");
		sentence = sentence.substr(8);                     // Remove <s> <s>
		sentence = sentence.substr(0, sentence.size() - 5);// Remove </s>
		sentence.erase(sentence.begin() + 11);
		sentence.erase(sentence.end() - 2);
		CHECK(msg == sentence);// Small corpus == Same sentence
	}
};
