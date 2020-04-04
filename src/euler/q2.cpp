// Headers
#include "euler/euler.hpp"

namespace ruff
{
	namespace euler
	{
		std::vector<unsigned int> generateFibonacci(const unsigned int max,
				const unsigned int first,
				const unsigned int second)
		{
			std::vector<unsigned int> output;
			int index = 2;


			output.push_back(first);
			output.push_back(second);

			do
			{
				output.push_back(output[index-1] + output[index-2]);
			}while(output[index++] < max);

			return output;
		}

		unsigned int countEven(std::vector<unsigned int> numbers)
		{
			unsigned int sum = 0;
			for(const auto num : numbers)
			{
				if(num % 2 == 0)
				{
					sum += num;
				}
			}
			return sum;
		}
		
	}
}
