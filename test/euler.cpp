#include "doctest/doctest.h"
#include <iostream>

#include "euler/euler.hpp"
using namespace ruff::euler;

TEST_CASE("Euler question 1")
{
	CHECK(sumOfMultiples({ 3, 5 }, 0, 10) == 23);
	CHECK(sumOfMultiples({ 3, 5 }, 0, 1000) == 233168);
	CHECK(sumOfMultiples({ 3, 5 }, 0, 100000) == 2333316668);
}

TEST_CASE("Euler question 2")
{
	std::vector<unsigned int> fibSequence = generateFibonacci(4E6);
	unsigned int sum = countEven(fibSequence);
	CHECK(sum == 4613732);
	fibSequence = generateFibonacci(50);
	sum = countEven(fibSequence);
	CHECK(sum == 44);
}

TEST_CASE("Euler question 3")
{
	long int num = 13195;
	auto primes = findPrimeFactors(num);
	std::vector<int> correct{ 5, 7, 13, 29 };
	CHECK(primes == correct);

	num = 600851475143;
	primes = findPrimeFactors(num);
	correct = { 71, 839, 1471, 6857 };
	CHECK(primes == correct);
}

TEST_CASE("Euler question 6")
{
	unsigned int sum = sumOfSquares(1, 10);
	unsigned int square = squareOfSum(1, 10);
	CHECK((square - sum) == 2640);

	sum = sumOfSquares(1, 100);
	square = squareOfSum(1, 100);
	CHECK((square - sum) == 25164150);
}
