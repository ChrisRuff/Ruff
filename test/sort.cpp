#include "doctest/doctest.h"
#include <iostream>
#include <vector>

#include "sorting/sorting.hpp"

bool compareInts(int a, int b)
{
	return a > b;
}

TEST_CASE("Selection Sort")
{
	std::vector<int> list{1,4,2,3,8,7,5,6,9};

	ruff::sort::selectionSort<int>(list, &compareInts);
	for(size_t i = 0; i < list.size(); ++i)
	{
		CHECK(i+1 == list[i]);
	}
}
TEST_CASE("Bubble Sort")
{
	std::vector<int> list{1,4,2,3,8,7,5,6,9};

	ruff::sort::bubbleSort<int>(list, &compareInts);
	for(size_t i = 0; i < list.size(); ++i)
	{
		CHECK(i+1 == list[i]);
	}
}
TEST_CASE("Quick Sort")
{
	std::vector<int> list{1,4,2,3,8,7,5,6,9};

	ruff::sort::quickSort<int>(list, &compareInts);
	for(size_t i = 0; i < list.size(); ++i)
	{
		CHECK(i+1 == list[i]);
	}
}
