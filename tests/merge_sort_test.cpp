
#include <gtest/gtest.h>
#include <iostream>
#include <unordered_set>
#include <memory>
#include <chrono>
#include <vector>
#include <algorithm>
#include <random>
#include "sort.h"

/**
 * @brief MergeSortTest class
 * @details Test fixture for Merge Sort
 * 
 */
class MergeSortTest : public ::testing::Test {
protected:
    std::vector<int> randomNumbers_int {};
    std::vector<float> randomNumbers_flt {};
    
    // Helper function to compare two vectors for equality
    template <typename T>
    bool isSorted(const std::vector<T>& vec) const {
        for (size_t i = 1; i < vec.size(); ++i) {
            if (vec[i - 1] > vec[i]) return false;
        }
        return true;
    }
    
public:
    void SetUp() override {
        // Generate num random numbers between min and max
        int num = 25000;
        int imin = 1;
        int imax = 10000;
        float fmin = 1.0;
        float fmax = 10000.0;
    
        randomNumbers_int = algolab::generateRandomNumbers<int>(num, imin, imax);       
        randomNumbers_flt = algolab::generateRandomNumbers<float>(num, fmin, fmax);
    }
};
    
// Test sorting an unsorted vector
TEST_F(MergeSortTest, SortUnsortedVector) {
    std::vector<int> vec = {5, 3, 8, 1, 2};
    algolab::mergeSortAll<int>(vec);
    // Check that the vector is now sorted
    EXPECT_TRUE(isSorted<int>(vec));

    std::vector<float> vec2 = {4.32, 3.11, 43.21, 55.17, 24.59};
    algolab::mergeSortAll<float>(vec2);
    // Check that the vector is now sorted
    EXPECT_TRUE(isSorted<float>(vec2));
}
    
// Test sorting an already sorted vector
TEST_F(MergeSortTest, SortSortedVector) {
    std::vector<int> vec = {1, 2, 3, 5, 8};
    algolab::mergeSortAll<int>(vec);
    // Check that the vector is still sorted
    EXPECT_TRUE(isSorted<int>(vec));

    std::vector<float> vec2 = {0.111, 3.5635, 5.754, 7.3423, 9.989};
    algolab::mergeSortAll<float>(vec2);
    // Check that the vector is still sorted
    EXPECT_TRUE(isSorted<float>(vec2));
}

TEST_F(MergeSortTest, SortReverseOrderedVector) {
    std::vector<int> vec = {9, 7, 5, 3, 1};
    algolab::mergeSortAll<int>(vec);
    EXPECT_TRUE(isSorted(vec));
}
    
// Test sorting a vector with one element
TEST_F(MergeSortTest, SortSingleElement) {
    std::vector<int> vec = {42};
    algolab::mergeSortAll<int>(vec);
    
    // A single element is trivially sorted
    EXPECT_TRUE(isSorted(vec));
}
    
// Test sorting an empty vector
TEST_F(MergeSortTest, SortEmptyVector) {
    std::vector<int> vec = {};
    algolab::mergeSortAll<int>(vec);
    
    // An empty vector is trivially sorted
    EXPECT_TRUE(isSorted(vec));
}
    
// Test sorting a vector with duplicate elements
TEST_F(MergeSortTest, SortVectorWithDuplicates) {
    std::vector<int> vec = {4, 2, 5, 5, 1, 2, 3};
    algolab::mergeSortAll<int>(vec);
    // Check that the vector is sorted correctly, including duplicates
    EXPECT_TRUE(isSorted(vec));

    std::vector<float> vec2 = {5.43543, 2.1223, 4.45, 4.45, 1.212, 2.11, 3.87};
    algolab::mergeSortAll<float>(vec2);
    EXPECT_TRUE(isSorted(vec2));
}
    
TEST_F(MergeSortTest, SortRandomNumbers) {
    algolab::mergeSortAll<int>(randomNumbers_int);
    // Check that the vector is sorted correctly
    EXPECT_TRUE(isSorted(randomNumbers_int));

    algolab::mergeSortAll<float>(randomNumbers_flt);
    EXPECT_TRUE(isSorted(randomNumbers_flt));
}

TEST_F(MergeSortTest, /*DISABLED_*/BenchmarkSortRandom) {
    auto start = std::chrono::high_resolution_clock::now();
    algolab::mergeSortAll(randomNumbers_int);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "MergeSort (int) time: " 
              << std::chrono::duration<double, std::milli>(end - start).count()
              << " ms\n";
    
    start = std::chrono::high_resolution_clock::now();
    algolab::mergeSortAll(randomNumbers_flt);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "MergeSort (float) time: " 
              << std::chrono::duration<double, std::milli>(end - start).count()
              << " ms\n";
}
