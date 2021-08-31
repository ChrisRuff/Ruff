#include "doctest/doctest.h"
#include <iostream>
#include <vector>

#include "ruff/core/sorting.hpp"

TEST_SUITE("Sorting")
{
	TEST_CASE("Selection Sort")
	{
		std::vector<int> list{ 1, 4, 2, 3, 8, 7, 5, 6, 9 };

		ruff::sort::selectionSort(list);
		for(size_t i = 0; i < list.size(); ++i)
		{
			CHECK(i + 1 == list[i]);
		}
	}

	TEST_CASE("Insertion Sort")
	{
		std::vector<int> list{ 1, 4, 2, 3, 8, 7, 5, 6, 9 };

		ruff::sort::insertionSort(list);

		for(size_t i = 0; i < list.size(); ++i)
		{
			CHECK(i + 1 == list[i]);
		}
	}

	TEST_CASE("Bubble Sort")
	{
		std::vector<int> list{ 1, 4, 2, 3, 8, 7, 5, 6, 9 };

		ruff::sort::bubbleSort(list);
		for(size_t i = 0; i < list.size(); ++i)
		{
			CHECK(i + 1 == list[i]);
		}
	}
	TEST_CASE("Quick Sort")
	{
		std::vector<int> list{ 1, 4, 2, 3, 8, 7, 5, 6, 9 };

		ruff::sort::quickSort(list);
		for(size_t i = 0; i < list.size(); ++i)
		{
			CHECK(i + 1 == list[i]);
		}
	}
	TEST_CASE("Radix Sort")
	{
		std::vector<int> list{ 1, 4, 2, 3, 8, 7, 5, 6, 9 };

		ruff::sort::radixSort<int>(list, [](int a) { return a; });

		for(size_t i = 0; i < list.size(); ++i)
		{
			CHECK(i + 1 == list[i]);
		}
	}
	TEST_CASE("Merge Sort")
	{
		std::vector<int> list{ 1, 4, 2, 3, 8, 7, 5, 6, 9 };

		ruff::sort::mergeSort(list);

		for(size_t i = 0; i < list.size(); ++i)
		{
			CHECK(i + 1 == list[i]);
		}
	}
}
