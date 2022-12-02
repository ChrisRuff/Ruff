#pragma once
#include <ruff/nlp/gram_model.hpp>

namespace ruff::nlp
{

template<typename T, uint N, double K>
requires IsTokenizer<T>
class LaplaceGramModel : public GramModel<T, N>
{
public:
	LaplaceGramModel() : GramModel<T, N>()
	{
	}

	void AddKSmoothing()
	{
		for(auto& context : GramModel<T, N>::counts)
		{
			int total = 0;
			for(auto& word : context.second)
			{
				total += word.second;
			}
			for(auto& word : context.second)
			{
				word.second = (word.second + K) / (total + (K * GramModel<T, N>::types.size()));
			}
		}
	}

	std::string Choose(std::string_view sentence)
	{
		auto tokens = GramModel<T, N>::tokenizer.ParseLine(sentence);
		std::string context = "";
		if(tokens.size() < N - 1)
			throw(std::string("Must begin sentence with ") + std::to_string(N) + " <s>");
		for(int j = N - 1; j > 0; --j)
		{
			context += tokens[tokens.size() - j] + (j == 1 ? "" : " ");
		}
		auto count_map = GramModel<T, N>::counts[context];
		int total = 0;
		for(auto it = count_map.begin(); it != count_map.end(); ++it)
		{
			total += it->second;
		}
		double rand = GramModel<T, N>::dist(GramModel<T, N>::rng);
		double cumulativeProb = 0.0;
		for(auto type_it = GramModel<T, N>::types.begin(); type_it != GramModel<T, N>::types.end(); ++type_it)
		{
			if(count_map.contains(*type_it))
			{
				double prob = (count_map[*type_it] + K) / (total + K * GramModel<T, N>::types.size());
				cumulativeProb += prob;
			}
			else
			{
				double prob = (K / (total + K * GramModel<T, N>::types.size()));
				cumulativeProb += prob;
			}
			if(rand <= cumulativeProb && *type_it != "<s>")
			{
				return *type_it;
			}
		}
		std::cout << cumulativeProb << std::endl;
		return *GramModel<T, N>::types.rbegin();
	}
};
}// namespace ruff::nlp