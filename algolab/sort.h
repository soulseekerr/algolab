#pragma once

#include <iostream>
#include <vector>
#include <iostream>
#include <random>
#include <algorithm>
#include <type_traits>

namespace algolab {

// Function to generate a vector of random numbers
template <typename T>
std::vector<T> generateRandomNumbers(int num, T min, T max) {
    // Prevents misuse with non-numeric types
    static_assert(std::is_arithmetic_v<T>, "T must be an arithmetic type");

    std::vector<T> randomNumbers;
    randomNumbers.reserve(num);

    // Seed with a real random value, if available
    std::random_device rd;
    // Initialize the random number generator
    // More predictable and widely preferred engine than default_random_engine
    std::mt19937 eng(rd());

    // Define the range
    // Allows both integral and floating point generation (type-safe)
    if constexpr (std::is_integral_v<T>) {
        std::uniform_int_distribution<T> dist(min, max);
        for (int i = 0; i < num; ++i) {
            randomNumbers.push_back(dist(eng));
        }
    } else if constexpr (std::is_floating_point_v<T>) {
        std::uniform_real_distribution<T> dist(min, max);
        for (int i = 0; i < num; ++i) {
            randomNumbers.push_back(dist(eng));
        }
    }

    return randomNumbers;
}

// Function to perform bubble sort on a vector of integers
template <typename T>
void bubbleSort(std::vector<T>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        // Last i elements are already in place
        for (int j = 0; j < n - i - 1; ++j) {
            // Swap if the element found is greater than the next element
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// Function to perform selection sort on a vector of integers
template <typename T>
void selectionSort(std::vector<T>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        // Find the minimum element in the unsorted portion
        int minIndex = i;
        for (int j = i + 1; j < n; ++j) {
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }
        // Swap the found minimum element with the first unsorted element
        if (minIndex != i) {
            std::swap(arr[i], arr[minIndex]);
        }
    }
}

// MergeSort: Function to merge two sorted subarrays
template <typename T>
void mergeSortedHalves(std::vector<T>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Create temporary arrays
    std::vector<T> L(n1);
    std::vector<T> R(n2);

    // Copy data to temporary arrays L[] and R[]
    for (int i = 0; i < n1; ++i)
        L[i] = arr[left + i];
    for (int i = 0; i < n2; ++i)
        R[i] = arr[mid + 1 + i];

    // Merge the temporary arrays back into arr[left..right]

    int i = 0; // Initial index of first subarray
    int j = 0; // Initial index of second subarray
    int k = left; // Initial index of merged subarray

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            ++i;
        } else {
            arr[k] = R[j];
            ++j;
        }
        ++k;
    }

    // Copy the remaining elements of L[], if there are any
    while (i < n1) {
        arr[k] = L[i];
        ++i;
        ++k;
    }

    // Copy the remaining elements of R[], if there are any
    while (j < n2) {
        arr[k] = R[j];
        ++j;
        ++k;
    }
}

// Function to implement merge sort
// Recursively split the array into two halves, sort each half, 
// and then merge the sorted halves together. 
// The merge function performs the merging of two sorted subarrays.
template <typename T>
void mergeSort(std::vector<T>& arr, int left, int right) {
    if (left >= right) {
        return;
    }
    
    int mid = left + (right - left) / 2;

    // Sort first and second halves
    mergeSort<T>(arr, left, mid);
    mergeSort<T>(arr, mid + 1, right);

    // Merge the sorted halves
    mergeSortedHalves<T>(arr, left, mid, right);
}

template <typename T>
void mergeSortAll(std::vector<T>& arr) {
    mergeSort<T>(arr, 0, arr.size() - 1);
}

// Partition function to place the pivot element in its correct position
template <typename T>
int partition(std::vector<T>& arr, int low, int high) {
    T pivot = arr[high]; // Pivot element is taken as the last element
    int i = low - 1; // Index of smaller element

    for (int j = low; j <= high - 1; ++j) {
        // If current element is smaller than or equal to pivot
        if (arr[j] <= pivot) {
            ++i; // Increment index of smaller element
            std::swap(arr[i], arr[j]); // Swap the current element with the element at index i
        }
    }
    std::swap(arr[i + 1], arr[high]); // Place the pivot element in the correct position
    return i + 1;
}

// QuickSort function
template <typename T>
void quickSort(std::vector<T>& arr, int low, int high) {
    if (low >= high) {
        return;
    }
    
    // Partitioning index
    int pivotIndex = partition<T>(arr, low, high);

    // Recursively sort elements before and after partition
    quickSort<T>(arr, low, pivotIndex - 1);
    quickSort<T>(arr, pivotIndex + 1, high);
}

template <typename T>
void quickSortAll(std::vector<T>& arr) {
    if (!arr.empty()) {
        quickSort<T>(arr, 0, arr.size() - 1);
    }
}

// Function to heapify a subtree rooted with node i which is an index in arr[]
template <typename T>
void heapify(std::vector<T>& arr, int n, int i) {
    int largest = i; // Initialize largest as root
    int left = 2 * i + 1; // Left child
    int right = 2 * i + 2; // Right child

    // If left child is larger than root
    if (left < n && arr[left] > arr[largest])
        largest = left;

    // If right child is larger than largest so far
    if (right < n && arr[right] > arr[largest])
        largest = right;

    // If largest is not root
    if (largest != i) {
        std::swap(arr[i], arr[largest]);

        // Recursively heapify the affected sub-tree
        heapify<T>(arr, n, largest);
    }
}

// Main function to do heap sort
template <typename T>
void heapSort(std::vector<T>& arr) {
    int n = arr.size();

    // Build heap (rearrange array)
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify<T>(arr, n, i);

    // One by one extract an element from heap
    for (int i = n - 1; i > 0; i--) {
        // Move current root to end
        std::swap(arr[0], arr[i]);

        // Call max heapify on the reduced heap
        heapify<T>(arr, i, 0);
    }
}

} // namespace eden