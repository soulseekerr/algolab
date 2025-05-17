#pragma once

#include <vector>
#include <stack>
#include <iostream>

// Avoids deep recursion and stack overflow issues
// Better performance on large datasets by preventing function call overhead
// Ensures tail-recursive behavior, which compilers may not always optimize well in the recursive version

namespace algolab {

namespace sort_iter {

// Partition function for Quick Sort
template <typename T>
int partition(std::vector<T>& arr, int low, int high) {
    int mid = low + (high - low) / 2;
    
    // Median-of-Three: Pick median of {arr[low], arr[mid], arr[high]}
    // The median-of-three pivot selection prevents worst-case O(n²) behavior,
    // ensuring balanced partitions, and is O(n log n).
    if (arr[mid] < arr[low]) std::swap(arr[mid], arr[low]);
    if (arr[high] < arr[low]) std::swap(arr[high], arr[low]);
    if (arr[mid] < arr[high]) std::swap(arr[mid], arr[high]);

    T pivot = arr[high]; // New pivot
    int i = low - 1;

    for (int j = low; j < high; ++j) {
        if (arr[j] <= pivot) {
            std::swap(arr[++i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

// Iterative Quick Sort function
template <typename T>
void quickSortAll(std::vector<T>& arr) {
    if (arr.empty()) return;

    // vector slightly more efficient than stack (based on deque)
    // using push_back and pop_back improves cache locality
    std::vector<std::pair<int, int>> stack;
    stack.push_back({0, static_cast<int>(arr.size() - 1)});

    while (!stack.empty()) {
        auto [low, high] = stack.back();
        stack.pop_back();

        if (low < high) {
            int pivotIndex = partition(arr, low, high);

            // Push left subarray
            if (pivotIndex - 1 > low) {
                stack.push_back({low, pivotIndex - 1});
            }
            // Push right subarray
            if (pivotIndex + 1 < high) {
                stack.push_back({pivotIndex + 1, high});
            }
        }
    }
}

} // namespace sort_iter

} // namespace algolab