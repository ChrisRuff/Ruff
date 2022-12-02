#pragma once
#include <concepts>
#include <string_view>
#include <unordered_map>
#include <set>
#include <random>

#include <cstdlib>
#include <ctime>

#include <ruff/nlp/tokenizer.hpp>

namespace ruff::nlp
{
using CountMatrix = std::unordered_map<
  std::string,
  std::unordered_map<std::string, float>>;

template<typename T, uint N>
requires IsTokenizer<T>
class GramModel
{
protected:
	std::random_device rng_device{};
	std::mt19937 rng{ rng_device() };
	std::uniform_real_distribution<> dist{ 0.0, 1.0 };

	T tokenizer;
	CountMatrix counts{};
	std::set<std::string> types{};

	int total_tokens{};

public:
	GramModel() : tokenizer()
	{
	}
	virtual ~GramModel() = default;
	std::string GetStarter()
	{
		std::string out = "";
		for(uint i = 0; i < N - 1; ++i)
		{
			out += std::string("<s>") + (i == N - 2 ? "" : " ");
		}
		return out;
	}
	void ReadLine(std::string_view line, bool preprocess = true)
	{
		std::vector<std::string> tokens = tokenizer.ParseLine(line);
		if(preprocess)
		{
			for(uint i = 0; i < N - 1; ++i)
			{
				tokens.insert(tokens.begin(), "<s>");
			}
			tokens.push_back("</s>");
		}
		total_tokens += tokens.size();
		for(size_t i = 0; i < tokens.size(); ++i)
		{
			types.emplace(tokens[i]);
		}
		for(size_t i = N - 1; i < tokens.size(); ++i)
		{
			std::string context = "";
			for(int j = N - 1; j > 0; --j)
			{
				context += tokens[i - j] + (j == 1 ? "" : " ");
			}
			counts[context][tokens[i]]++;
		}
	}
	virtual std::string Choose(std::string_view sentence)
	{
		auto tokens = tokenizer.ParseLine(sentence);
		std::string context = "";
		if(tokens.size() < N - 1)
			throw(std::string("Must begin sentence with ") + std::to_string(N) + " <s>");
		for(int j = N - 1; j > 0; --j)
		{
			context += tokens[tokens.size() - j] + (j == 1 ? "" : " ");
		}
		auto count_map = counts[context];
		int total = 0;
		for(auto it = count_map.begin(); it != count_map.end(); ++it)
		{
			total += it->second;
		}
		std::srand(std::time(nullptr));
		double rand = (static_cast<double>(std::rand()) / (RAND_MAX));

		double cumulativeProb = 0.0;
		for(auto it = count_map.begin(); it != count_map.end(); ++it)
		{
			cumulativeProb += it->second / total;
			if(rand <= cumulativeProb && it->first != "<s>")
				return it->first;
		}
		return "";
	}
};
}// namespace ruff::nlp
