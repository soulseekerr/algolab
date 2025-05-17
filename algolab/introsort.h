#pragma once

#include <vector>
#include <algorithm>
#include <cmath>

namespace algolab {

/**
 * @brief Introsort implementation
 * @details
 * Introsort is a hybrid sorting algorithm that combines the best features of
 * QuickSort, HeapSort, and InsertionSort. It begins with QuickSort and switches
 * to HeapSort when the recursion depth exceeds a certain limit. For small
 * partitions, it uses InsertionSort for better performance.
 * 
 * Implementation of Introsort under algolab::sort_custom::introSortAll.
 * It switches between:
 * QuickSort (with partition)
 * HeapSort (when recursion depth is too high)
 * InsertionSort (for small partitions)
 */ 

namespace sort_custom {

// Insertion Sort for small ranges
template <typename T>
void insertionSort(std::vector<T>& arr, int low, int high) {
    for (int i = low + 1; i <= high; ++i) {
        T key = arr[i];
        int j = i - 1;
        while (j >= low && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

// Median-of-Three pivot selection
template <typename T>
int medianOfThree(std::vector<T>& arr, int low, int high) {
    int mid = low + (high - low) / 2;
    if (arr[high] < arr[low]) std::swap(arr[low], arr[high]);
    if (arr[mid] < arr[low]) std::swap(arr[mid], arr[low]);
    if (arr[high] < arr[mid]) std::swap(arr[high], arr[mid]);
    std::swap(arr[mid], arr[high - 1]); // Place pivot at high - 1
    return high - 1;
}

// Partition function (Lomuto-style)
// Could use median-of-three pivot or Hoare partitioning instead of Lomuto
// template <typename T>
// int partition_lomuto(std::vector<T>& arr, int low, int high) {
//     T pivot = arr[high];
//     int i = low - 1;
//     for (int j = low; j < high; ++j) {
//         if (arr[j] < pivot) {
//             ++i;
//             std::swap(arr[i], arr[j]);
//         }
//     }
//     std::swap(arr[i + 1], arr[high]);
//     return i + 1;
// }

// Partition function using median-of-three
template <typename T>
int partition(std::vector<T>& arr, int low, int high) {
    int pivotIndex = medianOfThree(arr, low, high);
    T pivot = arr[pivotIndex];
    int i = low;
    int j = high - 1;

    while (true) {
        while (arr[++i] < pivot) {}
        while (arr[--j] > pivot) {}
        if (i < j) {
            std::swap(arr[i], arr[j]);
        } else {
            break;
        }
    }
    std::swap(arr[i], arr[high - 1]); // Restore pivot
    return i;
}

// HeapSort for worst-case scenarios
template <typename T>
void heapSort(std::vector<T>& arr, int low, int high) {
    std::make_heap(arr.begin() + low, arr.begin() + high + 1);
    std::sort_heap(arr.begin() + low, arr.begin() + high + 1);
}

// Introsort core logic
template <typename T>
void introsort(std::vector<T>& arr, int low, int high, int depthLimit) {
    const int sizeThreshold = 16;

    if (high - low <= sizeThreshold) {
        insertionSort(arr, low, high);
        return;
    }

    if (depthLimit == 0) {
        heapSort(arr, low, high);
        return;
    }

    int pivotIndex = partition(arr, low, high);
    introsort(arr, low, pivotIndex - 1, depthLimit - 1);
    introsort(arr, pivotIndex + 1, high, depthLimit - 1);
}

// Public interface
template <typename T>
void introSortAll(std::vector<T>& arr) {
    int n = static_cast<int>(arr.size());
    int depthLimit = 2 * static_cast<int>(std::log2(n));
    introsort(arr, 0, n - 1, depthLimit);
}

} // namespace sort_custom

} // namespace algolab
