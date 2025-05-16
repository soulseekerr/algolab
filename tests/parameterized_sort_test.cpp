
#include <gtest/gtest.h>
#include <iostream>
#include <unordered_set>
#include <memory>
#include <chrono>
#include <vector>
#include <algorithm>
#include <random>
#include "sort.h"


// Optional wrapper to standardize std::sort interface
template <typename T>
void stdSortWrapper(std::vector<T>& vec) {
    std::sort(vec.begin(), vec.end());
}

using SortFunctionInt = std::function<void(std::vector<int>&)>;
using SortFunctionFloat = std::function<void(std::vector<float>&)>;

struct NamedSortInt {
    std::string name;
    SortFunctionInt func;
};

struct NamedSortFloat {
    std::string name;
    SortFunctionFloat func;
};

// Test fixture for parameterized sorting algorithm tests
template <typename Type, typename NamedSortType>
class SortingParameterizedTest: public ::testing::TestWithParam<NamedSortType> {
protected:
    std::vector<Type> randomNumbers {};
        
    // Helper function to compare two vectors for equality
    bool isSorted(const std::vector<Type>& vec) const {
        for (size_t i = 1; i < vec.size(); ++i) {
            if (vec[i - 1] > vec[i]) return false;
        }
        return true;
    }
    
    bool stdIsSorted(const std::vector<Type>& vec) const {
        return std::is_sorted(vec.begin(), vec.end());
    }
    
    virtual std::vector<Type> generateSample() const = 0;
    virtual std::vector<Type> generateRandom() const = 0;
};
    
class SortingParameterizedTestInt: public SortingParameterizedTest<int, NamedSortInt> {
protected:
    std::vector<int> generateSample() const override {
        return {5, 2, 9, 1, 5, 6};
    }

    std::vector<int> generateRandom() const override {
        return algolab::generateRandomNumbers<int>(50000, 0, 10000);
    }
};

class SortingParameterizedTestFloat: public SortingParameterizedTest<float, NamedSortFloat> {
protected:
    std::vector<float> generateSample() const override {
        return {5.23f, 2.1f, 9.7f, 1.342f, 5.044f, 6.456f};
    }
    
    std::vector<float> generateRandom() const override {
        return algolab::generateRandomNumbers<float>(50000, 0.0f, 10000.0f);
    }
};

TEST_P(SortingParameterizedTestInt, SortsSampleCorrectly) {
    std::vector<int> vec = generateSample();
    auto sort = GetParam().func;
    sort(vec);
    EXPECT_TRUE(isSorted(vec));
}

TEST_P(SortingParameterizedTestInt, SortsRandomCorrectly) {
    std::vector<int> vec = generateRandom();
    auto sort = GetParam().func;
    sort(vec);
    EXPECT_TRUE(isSorted(vec));
}

TEST_P(SortingParameterizedTestInt, HandlesEmptyVector) {
    std::vector<int> vec;
    auto sort = GetParam().func;
    sort(vec);
    EXPECT_TRUE(isSorted(vec));
}


TEST_P(SortingParameterizedTestInt, HandlesSingleElement) {
    std::vector<int> vec = {42};
    auto sort = GetParam().func;
    sort(vec);
    EXPECT_TRUE(isSorted(vec));
}

TEST_P(SortingParameterizedTestInt, HandlesAlreadySorted) {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    auto sort = GetParam().func;
    sort(vec);
    EXPECT_TRUE(isSorted(vec));
}

TEST_P(SortingParameterizedTestInt, HandlesReverseSorted) {
    std::vector<int> vec = {9, 7, 5, 3, 1};
    auto sort = GetParam().func;
    sort(vec);
    EXPECT_TRUE(isSorted(vec));
}

// ---- Instantiation ---- //

template<typename NamedSortType>
std::string NameFromStruct(const ::testing::TestParamInfo<NamedSortType>& info) {
    return info.param.name;
}

INSTANTIATE_TEST_SUITE_P(
    SortImplementations,
    SortingParameterizedTestInt,
    ::testing::Values(
        NamedSortInt{"BubbleSort", algolab::bubbleSort<int>},
        NamedSortInt{"MergeSort", algolab::mergeSortAll<int>},
        NamedSortInt{"QuickSort", algolab::quickSortAll<int>},
        NamedSortInt{"SelectionSort", algolab::selectionSort<int>},
        NamedSortInt{"HeapSort", algolab::heapSort<int>},
        NamedSortInt{"StdSort", stdSortWrapper<int>}
    ),
    NameFromStruct<NamedSortInt>
);


TEST_P(SortingParameterizedTestFloat, SortsSampleCorrectly) {
    std::vector<float> vec = generateSample();
    auto sort = GetParam().func;
    sort(vec);
    EXPECT_TRUE(isSorted(vec));
}

TEST_P(SortingParameterizedTestFloat, SortsRandomCorrectly) {
    std::vector<float> vec = generateRandom();
    auto sort = GetParam().func;
    sort(vec);
    EXPECT_TRUE(isSorted(vec));
}

TEST_P(SortingParameterizedTestFloat, HandlesEmptyVector) {
    std::vector<float> vec;
    auto sort = GetParam().func;
    sort(vec);
    EXPECT_TRUE(isSorted(vec));
}


TEST_P(SortingParameterizedTestFloat, HandlesSingleElement) {
    std::vector<float> vec = {42.2};
    auto sort = GetParam().func;
    sort(vec);
    EXPECT_TRUE(isSorted(vec));
}

TEST_P(SortingParameterizedTestFloat, HandlesAlreadySorted) {
    std::vector<float> vec = {1.2, 2.1, 3.4, 4.7, 5.9};
    auto sort = GetParam().func;
    sort(vec);
    EXPECT_TRUE(isSorted(vec));
}

TEST_P(SortingParameterizedTestFloat, HandlesReverseSorted) {
    std::vector<float> vec = {9.34f, 7.1f, 5.044f, 3.3f, 1.201f};
    auto sort = GetParam().func;
    sort(vec);
    EXPECT_TRUE(isSorted(vec));
}

// ---- Instantiation ---- //

INSTANTIATE_TEST_SUITE_P(
    SortImplementations,
    SortingParameterizedTestFloat,
    ::testing::Values(
        NamedSortFloat{"BubbleSort", algolab::bubbleSort<float>},
        NamedSortFloat{"MergeSort", algolab::mergeSortAll<float>},
        NamedSortFloat{"QuickSort", algolab::quickSortAll<float>},
        NamedSortFloat{"SelectionSort", algolab::selectionSort<float>},
        NamedSortFloat{"HeapSort", algolab::heapSort<float>},
        NamedSortFloat{"StdSort", stdSortWrapper<float>}
    ),
    NameFromStruct<NamedSortFloat>
);