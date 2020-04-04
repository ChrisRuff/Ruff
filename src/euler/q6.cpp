#include "euler/euler.hpp"

namespace ruff
{
	namespace euler
	{
		unsigned int sumOfSquares(const std::vector<int>& numbers)
		{
			unsigned int sum = 0;
			for(const int number : numbers)
			{
				sum += number * number;
			}
			return sum;
		}

		unsigned int sumOfSquares(int min, int max, int step)
		{
			std::vector<int> input;
			input.reserve((max-min)/step);
			for(; min <= max; min+=step)
			{
				input.push_back(min);
			}
			return sumOfSquares(input);
		}

		unsigned int squareOfSum(const std::vector<int>& numbers)
		{
			unsigned int sum = std::accumulate(numbers.begin(), numbers.end(), 0);
			return std::pow(sum, 2);
		}

		unsigned int squareOfSum(int min, int max, int step)
		{
			std::vector<int> input;
			input.reserve((max-min)/step);
			for(; min <= max; min+=step)
			{
				input.push_back(min);
			}
			return squareOfSum(input);
		}
	}
}

