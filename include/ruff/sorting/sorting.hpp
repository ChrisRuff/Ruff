#include <vector>

namespace ruff
{
namespace sort
{
	// Swap two elements in a vector
	template<typename T>
	inline void swap(std::vector<T>& list, int i, int j)
	{
		T t = list[i];
		list[i] = list[j];
		list[j] = t;
	}

	// Use selection sort algorithm to sort a list in-place
	// O(n^2)
	template<typename T>
	void selectionSort(std::vector<T>& list, bool (*comparator)(T, T))
	{
		// iterate through each element in the list
		int size = list.size();
		for(int i = 0; i < size-1; ++i)
		{
			// find the minimum element 
			int min = i;
			for(int j = i+1; j < size; ++j)
			{
				if(comparator(list[min], list[j]))
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

	// Use bubble sort algorithm to sort a list in-place
	// O(n^2)
	template<typename T>
	void bubbleSort(std::vector<T>& list, bool (*comparator)(T, T))
	{
		// Iterate through each element in the list
		int size = list.size();
		for(int i = 0; i < size-1; ++i)
		{
			// Find the maximum element and place it at 
			// the end, minus the amount of already found elements
			for(int j = 0; j < size-i-1; ++j)
			{
				if(comparator(list[j], list[j+1]))
				{
					swap<T>(list, j, j+1);
				}
			}
		}
	}

	template<typename T>
	int partition(std::vector<T>& list, int low, int high, bool (*comparator)(T, T))
	{
		int pivot = list[high];

		int i = low-1;
		for(int j = low; j < high; ++j)
		{
			if(comparator(pivot, list[j]))
			{
				++i;
				swap<T>(list, i, j);
			}
		}
		swap<T>(list, i+1, high);
		return i+1;
	}

	template<typename T>
	void quickSortStep(std::vector<T>& list, int low, int high, bool (*comparator)(T, T))
	{
		if(low < high)
		{
			int pivot = partition(list, low, high, comparator);
			quickSortStep(list, low, pivot-1, comparator);
			quickSortStep(list, pivot+1, high, comparator);
		}
	}
	template<typename T>
	void quickSort(std::vector<T>& list, bool (*comparator)(T, T))
	{
		quickSortStep(list, 0, list.size()-1, comparator);
	}

};
};
