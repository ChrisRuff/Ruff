// STL Packages
#include <numeric>

// Headers
#include "euler/euler.hpp"

namespace ruff
{
	namespace euler
	{
		unsigned int sumOfMultiples(const std::initializer_list<unsigned int>& multiples,
				const unsigned int lowerBound,
				const unsigned int upperBound)
		{
			std::unordered_set<unsigned int> numbers;
			numbers.reserve((upperBound-lowerBound) / multiples.size());

			unsigned int sum = 0;
			for(const unsigned int number: multiples)
			{
				for(std::size_t i = lowerBound; i < upperBound; i += number)
				{
					if(numbers.emplace(i).second)
					{
						sum += i;
					}

				}
			}
			return sum;
		}
	}
}
