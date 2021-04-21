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

	// ----------------------------- SELECTION SORT -------------------------------
	/* --------------------------------------------------------------------------*/
	/**
	 * @Synopsis  Use selection sort algorithm to sort a list in-place
	 *            Complexity: O(n^2)
	 * @Param list
	 */
	/* ----------------------------------------------------------------------------*/
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
	// 
	// 
	// 
	/* --------------------------------------------------------------------------*/
	/**
	 * @Synopsis  Fastest O(n^2) algorithm, iterates through the array and 
	 *            moves each element back until all elements to the left are sorted
	 *            Complexity: O(n^2)
	 * @Param list
	 */
	/* ----------------------------------------------------------------------------*/
	template<typename T> requires std::totally_ordered<T> 
	void insertionSort(std::vector<T>& list)
	{
		const size_t size = list.size();

		for(size_t i = 0; i < size; ++i)
		{
			// Scan to left and move this element until the previous
			// element is less than it
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
	// 
	// 
	// 
	/* --------------------------------------------------------------------------*/
	/**
	 * @Synopsis  Scans through list, taking the largest element and moves it
	 *            to the right until the max element is at the right.
	 *            Complexity: O(n^2)
	 *
	 * @Param list
	 */
	/* ----------------------------------------------------------------------------*/
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
	/* --------------------------------------------------------------------------*/
	/**
	 * @Synopsis  Picks a value, and moves all the elements greater than that 
	 *            value to the right of it, and all the elements less than it to the left
	 *
	 * @Param list List to be sorted
	 * @Param low lower index
	 * @Param high higher index
	 *
	 * @Returns New pivot
	 */
	/* ----------------------------------------------------------------------------*/
	template<typename T> requires std::totally_ordered<T> 
	int partition(std::vector<T>& list, const int low, const int high) 
	{
		// pick value to split the data
		const int pivot = list[high];

		int i = low - 1;
		for(int j = low; j < high; ++j)
		{
			// If the value is greater than the pivot then swap it
			// and the value at i
			if(pivot > list[j])
			{
				++i;
				swap<T>(list, i, j);
			}
		}
		swap<T>(list, i + 1, high);

		// Return new middle value
		return i + 1;
	}

	/* --------------------------------------------------------------------------*/
	/**
	 * @Synopsis  Recursively split the list into halves until they are one in size
	 *
	 * @Param list List to be sorted
	 * @Param low smallest index of the region to be sorted
	 * @Param high largest index of the region to be sorted
	 */
	/* ----------------------------------------------------------------------------*/
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

		/* --------------------------------------------------------------------------*/
		/**
		 * @Synopsis  Quick sort algorithm
		 *            O(nlogn) best case
		 *
		 * @Param list
		 */
		/* ----------------------------------------------------------------------------*/
	template<typename T> requires std::totally_ordered<T> 
	void quickSort(std::vector<T>& list) 
	{
		// Initiate recursion
		quickSortStep(list, 0, list.size() - 1);
	}

	// -------------------- RADIX SORT --------------------
	
	/* --------------------------------------------------------------------------*/
	/**
	 * @Synopsis  Base used in radix sorting
	 */
	/* ----------------------------------------------------------------------------*/
	constexpr int radix = 10;

	/* --------------------------------------------------------------------------*/
	/**
	 * @Synopsis  Find Max value in list 
	 *
	 * @Param list List to be sorted
	 * @Param const getID Function that can return the ID of the templated object
	 *
	 * @Returns   
	 */
	/* ----------------------------------------------------------------------------*/
	template<typename T>
	T getMax(std::vector<T>& list, int (*const getID)(T))
	{
		size_t size = list.size();

		// Variables to track max value
		int max = getID(list[0]);
		T maxEl = list[0];

		// Go through list and find max value
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

	/* --------------------------------------------------------------------------*/
	/**
	 * @Synopsis  Radix sort, sorts each element by it's id, placing
	 *            them into buckets based on their base, run the number of
	 *            digits in the max number
	 *                                                                     
	 *            Complexity: O(digits * (n + base))
	 *
	 * @Param list List to be sorted
	 * @Param const getID Function to return the id of the templated object
	 */
	/* ----------------------------------------------------------------------------*/
	template<typename T>
	void radixSort(std::vector<T>& list, int (*const getID)(T)) 
	{
		const size_t size = list.size();

		// Find the max value
		int max = getID(getMax(list, getID));

		// Operate for each digit in the number
		for(int exp = 1; max / exp > 0; exp *= radix)
		{
			// Create the bucket arrays
			std::vector<T> output(size);
			std::array<int, radix> count = { 0 };

			// Find the digit of the number
			for(size_t i = 0; i < size; ++i)
			{
				count[(getID(list[i]) / exp) % radix]++;
			}

			// Increase the index of count so that it reflects
			// the values in the array
			for(size_t i = 1; i < radix; ++i)
			{
				count.at(i) += count.at(i - 1);
			}

			// Place values into the output based on 
			// the count in the count array
			for(int i = size - 1; i >= 0; --i)
			{
				output[count[(getID(list[i]) / exp) % radix] - 1] = list[i];
				count[(getID(list[i]) / exp) % radix]--;
			}

			// Place output array into the list for next iteration
			for(size_t i = 0; i < size; ++i)
			{
				list[i] = output[i];
			}
		}
	}

	// --------------------------------- MERGE SORT -----------------------------
	/* --------------------------------------------------------------------------*/
	/**
	 * @Synopsis  Divide a region in two and sort it comparatively

	 *
	 * @Param list List to be sorted
	 * @Param l left index of sub list
	 * @Param m middle index of sub list
	 * @Param r right index of sub list
	 */
	/* ----------------------------------------------------------------------------*/
	template<typename T> requires std::totally_ordered<T> 
	void merge(std::vector<T>& list, const int l, const int m, const int r) 
	{
		// Start indices for left and right
		const size_t n1 = m - l + 1;
		const size_t n2 = r - m;

		// Fill left side array
		std::vector<T> L(n1);
		for(size_t i = 0; i < n1; ++i)
		{
			L[i] = list[l + i];
		}

		// Fill right side array
		std::vector<T> R(n1);
		for(size_t i = 0; i < n2; ++i)
		{
			R[i] = list[m + 1 + i];
		}

		// index vars
		size_t i = 0;
		size_t j = 0;
		size_t k = l;

		// Fill the list with the appropriate values
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

		// Fill the remaining values
		while(i < n1)
		{
			list[k++] = L[i++];
		}

		while(j < n2)
		{
			list[k++] = R[j++];
		}
	}

	/* --------------------------------------------------------------------------*/
	/**
	 * @Synopsis  Split list in two, recursively and merge the splits lists together
	 *
	 * @Param list List to be sorted
	 * @Param l leftmost index
	 * @Param r rightmost index
	 */
	/* ----------------------------------------------------------------------------*/
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

	/* --------------------------------------------------------------------------*/
	/**
	 * @Synopsis  Initiate Merge Sort algorithm
	 *            O(nlogn)
	 *
	 * @Param list List to be sorted
	 */
	/* ----------------------------------------------------------------------------*/
	template<typename T> requires std::totally_ordered<T> 
	void mergeSort(std::vector<T>& list) 
	{
		mergeSortStart(list, 0, list.size() - 1);
	}

};// namespace sort
};// namespace ruff
