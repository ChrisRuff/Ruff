#include <filesystem>
#include <fstream>

#include <ruff/nlp/simple_tokenizer.hpp>
#include <ruff/nlp/laplace_gram_model.hpp>
#include <iostream>

int main(int argc, char* argv[])
{
		std::filesystem::path data_dir("data");

		ruff::nlp::GramModel<ruff::nlp::SimpleTokenizer, 5> model;
		for(auto file : std::filesystem::directory_iterator(data_dir))
		{
				std::ifstream in_stream(file.path());
				std::string line;
				while(std::getline(in_stream, line))
				{
						model.ReadLine(line, true);
				}
		}

		std::string sentence = model.GetStarter();
		std::string word;
		do {
				word = model.Choose(sentence);
				sentence += " " + word;
		} while(word != "</s>");
		std::cout << sentence << std::endl;
}