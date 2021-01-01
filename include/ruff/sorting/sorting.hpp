#include <vector>
#include <array>
#include <concepts>

#include <logger.hpp>

namespace ruff
{
namespace sort
{
	// Swap two elements in a vector
	template<typename T>
	inline void swap(std::vector<T>& list, const int i, const int j)
	{
		T t = list[i];
		list[i] = list[j];
		list[j] = t;
	}

	// ----------------- SELECTION SORT ------------------
	// Use selection sort algorithm to sort a list in-place
	// O(n^2)
	template<typename T> requires std::totally_ordered<T> 
	void selectionSort(std::vector<T>& list)
	{
		// iterate through each element in the list
		int size = list.size();
		for(int i = 0; i < size - 1; ++i)
		{
			// find the minimum element
			int min = i;
			for(int j = i + 1; j < size; ++j)
			{
				if(list[min] > list[j])
				{
					min = j;
				}
			}
			// If a new minimum was found then swap this
			// and the minimum element
			if(i != min)
			{
				swap<T>(list, i, min);
			}
		}
	}

	// ----------------- INSERTION SORT -------------------
	template<typename T> requires std::totally_ordered<T> 
	void insertionSort(std::vector<T>& list)
	{
		const size_t size = list.size();
		for(size_t i = 0; i < size; ++i)
		{
			const T key = list[i];
			int j = static_cast<int>(i) - 1;

			while(j >= 0 && list[j] > key)
			{
				list[j + 1] = list[j];
				j--;
			}
			list[j + 1] = key;
		}
	}

	// ------------------- BUBBLE SORT --------------------
	// Use bubble sort algorithm to sort a list in-place
	// O(n^2)
	template<typename T> requires std::totally_ordered<T> 
	void bubbleSort(std::vector<T>& list)
	{
		// Iterate through each element in the list
		int size = list.size();
		for(int i = 0; i < size - 1; ++i)
		{
			// Find the maximum element and place it at
			// the end, minus the amount of already found elements
			for(int j = 0; j < size - i - 1; ++j)
			{
				if(list[j] > list[j + 1])
				{
					swap<T>(list, j, j + 1);
				}
			}
		}
	}

	// -------------------- QUICK SORT --------------------
	//
	template<typename T> requires std::totally_ordered<T> 
	int partition(std::vector<T>& list, const int low, const int high)
	{
		int pivot = list[high];

		int i = low - 1;
		for(int j = low; j < high; ++j)
		{
			if(pivot > list[j])
			{
				++i;
				swap<T>(list, i, j);
			}
		}
		swap<T>(list, i + 1, high);
		return i + 1;
	}

	template<typename T> requires std::totally_ordered<T> 
	void quickSortStep(std::vector<T>& list, const int low, const int high)
	{
		if(low < high)
		{
			const int pivot = partition(list, low, high);
			quickSortStep(list, low, pivot - 1);
			quickSortStep(list, pivot + 1, high);
		}
	}
	template<typename T> requires std::totally_ordered<T> 
	void quickSort(std::vector<T>& list)
	{
		quickSortStep(list, 0, list.size() - 1);
	}

	// -------------------- RADIX SORT --------------------
	template<typename T>
	T getMax(std::vector<T>& list, int (*const getID)(T))
	{
		int max = getID(list[0]);
		T maxEl = list[0];
		size_t size = list.size();
		for(size_t i = 1; i < size; ++i)
		{
			int id = getID(list[i]);
			if(max < id)
			{
				max = id;
				maxEl = list[i];
			}
		}
		return maxEl;
	}

	constexpr int radix = 10;

	template<typename T>
	void radixSort(std::vector<T>& list, int (*const getID)(T))
	{
		int max = getID(getMax(list, getID));
		const size_t size = list.size();

		for(int exp = 1; max / exp > 0; exp *= radix)
		{
			std::vector<T> output(size);
			std::array<int, radix> count = { 0 };

			for(size_t i = 0; i < size; ++i)
			{
				count[(getID(list[i]) / exp) % radix]++;
			}

			for(size_t i = 1; i < radix; ++i)
			{
				count.at(i) += count.at(i - 1);
			}
			for(int i = size - 1; i >= 0; --i)
			{
				output[count[(getID(list[i]) / exp) % radix] - 1] = list[i];
				count[(getID(list[i]) / exp) % radix]--;
			}
			for(size_t i = 0; i < size; ++i)
			{
				list[i] = output[i];
			}
		}
	}

	// -------------------- MERGE SORT --------------------
	template<typename T> requires std::totally_ordered<T> 
	void merge(std::vector<T>& list, const int l, const int m, const int r)
	{
		const size_t n1 = m - l + 1;
		const size_t n2 = r - m;

		std::vector<T> L(n1);
		for(size_t i = 0; i < n1; ++i)
		{
			L[i] = list[l + i];
		}

		std::vector<T> R(n1);
		for(size_t i = 0; i < n2; ++i)
		{
			R[i] = list[m + 1 + i];
		}

		size_t i = 0;
		size_t j = 0;
		size_t k = l;

		while(i < n1 && j < n2)
		{
			if(L[i] < R[j])
			{
				list[k] = L[i++];
			}
			else
			{
				list[k] = R[j++];
			}
			++k;
		}

		while(i < n1)
		{
			list[k++] = L[i++];
		}

		while(j < n2)
		{
			list[k++] = R[j++];
		}
	}

	template<typename T> requires std::totally_ordered<T> 
	void mergeSortStart(std::vector<T>& list, const int l, const int r)
	{
		if(l >= r)
		{
			return;
		}
		int m = (l + (r - 1)) / 2;
		mergeSortStart(list, l, m);
		mergeSortStart(list, m + 1, r);
		merge(list, l, m, r);
	}

	template<typename T> requires std::totally_ordered<T> 
	void mergeSort(std::vector<T>& list)
	{
		mergeSortStart(list, 0, list.size() - 1);
	}
};// namespace sort
};// namespace ruff
