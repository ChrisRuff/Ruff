// STL Packages
#include <initializer_list>
#include <unordered_set>
#include <numeric>
#include <vector>
#include <iostream>
#include <array>
#include <cmath>

namespace ruff
{
	namespace euler
	{
		/* --------------------------------------------------------------------------*/
		/**
		 * @Synopsis  Finds the sum of all numbers that are multiples of the inputted 
		 * initializer list values within the upper and lower bounds
		 *
		 * @Param multiples Multiples of these numbers will be summed within the bounds
		 * @Param lowerBound 
		 * @Param upperBound
		 *
		 * @Returns   
		 */
		/* ----------------------------------------------------------------------------*/
		unsigned int sumOfMultiples(const std::initializer_list<unsigned int>& multiples,
				const unsigned int lowerBound = 0,
				const unsigned int upperBound = 1000);


		/* --------------------------------------------------------------------------*/
		/**
		 * @Synopsis  Generates the fibonacci sequence up to but the max value
		 *
		 * @Param max Max value the fibonacci sequence will count to
		 * @Param first start value 1
		 * @Param second start value 2
		 *
		 * @Returns Fibonacci sequence 
		 */
		/* ----------------------------------------------------------------------------*/
		std::vector<unsigned int> generateFibonacci(const unsigned int max,
				const unsigned int first = 1,
				const unsigned int second = 2);


		/* --------------------------------------------------------------------------*/
		/**
		 * @Synopsis  Counts all even numbers in the vector
		 *
		 * @Param numbers vector of numbers
		 *
		 * @Returns Sum of all even numbers
		 */
		/* ----------------------------------------------------------------------------*/
		unsigned int countEven(std::vector<unsigned int> numbers);



		/* --------------------------------------------------------------------------*/
		/**
		 * @Synopsis  Finds the prime factors for a given number
		 *
		 * @Param num 
		 *
		 * @Returns   list of all prime factors that multiplied together result in num
		 */
		/* ----------------------------------------------------------------------------*/
		std::vector<int> findPrimeFactors(long num);

		/* --------------------------------------------------------------------------*/
		/**
		 * @Synopsis Generates all prime numbers up to limit
		 *
		 * @Param limit max number that will be checked by the sieve
		 *
		 * @Returns list of prime numbers
		 */
		/* ----------------------------------------------------------------------------*/
		std::vector<unsigned long> SieveOfAtkin(unsigned long limit);




		/* --------------------------------------------------------------------------*/
		/**
		 * @Synopsis Sums all values from min to max with a step, and then squares the output
		 *
		 * @Param min start value
		 * @Param max inclusing end value
		 * @Param step how much the step is from the min to max, i.e.
		 * a min and max value of 1 and 10, with a step of 3 will result in 
		 * (1 + 4 + 7 + 10)^2 = 22^2 = 484
		 *
		 * @Returns 
		 */
		/* ----------------------------------------------------------------------------*/
		unsigned int squareOfSum(int min, int max, int step = 1);

		/* --------------------------------------------------------------------------*/
		/**
		 * @Synopsis Takes a vector of integers and sums then returning the square of the sum
		 *
		 * @Param numbers vector of numbers to be summed then squared
		 *
		 * @Returns 
		 */
		/* ----------------------------------------------------------------------------*/
		unsigned int squareOfSum(const std::vector<int>& numbers);

		/* --------------------------------------------------------------------------*/
		/**
		 * @Synopsis Sums each value squared from min to max with a step
		 *
		 * @Param min start value
		 * @Param max inclusing end value
		 * @Param step how much the step is from the min to max, i.e.
		 * a min and max value of 1 and 10, with a step of 3 will result in 
		 * (1^2 + 4^2 + 7^2 + 10^2) = (1 + 16 + 49 + 100) = 166
		 *
		 * @Returns 
		 */
		/* ----------------------------------------------------------------------------*/
		unsigned int sumOfSquares(int min, int max, int step = 1);

		/* --------------------------------------------------------------------------*/
		/**
		 * @Synopsis Takes a vector of integers and squares each value, then sums all the values
		 *
		 * @Param numbers vector of numbers to be squared then summed
		 *
		 * @Returns 
		 */
		/* ----------------------------------------------------------------------------*/
		unsigned int sumOfSquares(const std::vector<int>& numbers);
	}
}
