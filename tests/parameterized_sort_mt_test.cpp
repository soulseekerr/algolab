
#include <gtest/gtest.h>
#include <iostream>
#include <unordered_set>
#include <memory>
#include <chrono>
#include <vector>
#include <algorithm>
#include <random>
#include <string>
#include "sort.h"
#include "sort_mt.h"
#include "benchmark_logger.h"

using chrono_time_point = std::chrono::high_resolution_clock::time_point;

inline void logTiming(const std::string& label, const chrono_time_point& start, const chrono_time_point& end) {
    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    BenchmarkLogger::get().log(label, ns);
}

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
class SortingParameterizedTestMt: public ::testing::TestWithParam<NamedSortType> {
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

    virtual std::vector<Type> generateRandom() const = 0;
};
    
class SortingParameterizedTestMtInt: public SortingParameterizedTestMt<int, NamedSortInt> {
protected:
    std::vector<int> generateRandom() const override {
        return algolab::generateRandomNumbers<int>(10000, 0, 10000);
    }
};

// class SortingParameterizedTestMtFloat: public SortingParameterizedTestMt<float, NamedSortFloat> {
// protected:    
//     std::vector<float> generateRandom() const override {
//         return algolab::generateRandomNumbers<float>(50000, 0.0f, 10000.0f);
//     }
// };

// TEST_P(SortingParameterizedTestMtInt, SortsRandomCorrectly) {
//      std::vector<int> vec = generateRandom();
//     auto sort = GetParam().func;
    
//     auto start = std::chrono::high_resolution_clock::now();
//     sort(vec);
//     auto end = std::chrono::high_resolution_clock::now();

//     logTiming(GetParam().name + " (SortsRandomCorrectly)", start, end);

//     EXPECT_TRUE(isSorted(vec));
// }

// TEST(SortMtDebugTest, RunSingleQuickSort) {
//     auto vec = algolab::generateRandomNumbers<int>(10000, 0, 10000);
//     algolab::sort_mt::quickSortAll(vec);
//     EXPECT_TRUE(std::is_sorted(vec.begin(), vec.end()));
// }

// TEST_P(SortingParameterizedTestMtInt, HandlesEmptyVector) {
//     std::vector<int> vec;
//     auto sort = GetParam().func;
    
//     auto start = std::chrono::high_resolution_clock::now();
//     sort(vec);
//     auto end = std::chrono::high_resolution_clock::now();

//     logTiming(GetParam().name + " (HandlesEmptyVector)", start, end);

//     EXPECT_TRUE(isSorted(vec));
// }

// TEST_P(SortingParameterizedTestMtInt, HandlesSingleElement) {
//     std::vector<int> vec = {42};
//     auto sort = GetParam().func;
    
//     auto start = std::chrono::high_resolution_clock::now();
//     sort(vec);
//     auto end = std::chrono::high_resolution_clock::now();

//     logTiming(GetParam().name + " (HandlesSingleElement)", start, end);

//     EXPECT_TRUE(isSorted(vec));
// }

// TEST_P(SortingParameterizedTestMtInt, HandlesAlreadySorted) {
//     std::vector<int> vec = {1, 2, 3, 4, 5};
//     auto sort = GetParam().func;
    
//     auto start = std::chrono::high_resolution_clock::now();
//     sort(vec);
//     auto end = std::chrono::high_resolution_clock::now();

//     logTiming(GetParam().name + " (HandlesAlreadySorted)", start, end);

//     EXPECT_TRUE(isSorted(vec));
// }

// TEST_P(SortingParameterizedTestMtInt, HandlesReverseSorted) {
//     std::vector<int> vec = {9, 7, 5, 3, 1};
//     auto sort = GetParam().func;
    
//     auto start = std::chrono::high_resolution_clock::now();
//     sort(vec);
//     auto end = std::chrono::high_resolution_clock::now();

//     logTiming(GetParam().name + " (HandlesReverseSorted)", start, end);

//     EXPECT_TRUE(isSorted(vec));
// }

// ---- Instantiation ---- //

template<typename NamedSortType>
std::string NameFromStruct(const ::testing::TestParamInfo<NamedSortType>& info) {
    return info.param.name;
}

// INSTANTIATE_TEST_SUITE_P(
//     SortImplementations,
//     SortingParameterizedTestMtInt,
//     ::testing::Values(
//         NamedSortInt{"QuickSortMt", algolab::sort_mt::quickSortAll<int>}
//     ),
//     NameFromStruct<NamedSortInt>
// );


// TEST_P(SortingParameterizedTestMtFloat, SortsRandomCorrectly) {
//     std::vector<float> vec = generateRandom();
//     auto sort = GetParam().func;
    
//     auto start = std::chrono::high_resolution_clock::now();
//     sort(vec);
//     auto end = std::chrono::high_resolution_clock::now();

//     logTiming(GetParam().name + " (SortsRandomCorrectly)", start, end);

//     EXPECT_TRUE(isSorted(vec));
// }

// TEST_P(SortingParameterizedTestMtFloat, HandlesEmptyVector) {
//     std::vector<float> vec;
//     auto sort = GetParam().func;
    
//     auto start = std::chrono::high_resolution_clock::now();
//     sort(vec);
//     auto end = std::chrono::high_resolution_clock::now();

//     logTiming(GetParam().name + " (HandlesEmptyVector)", start, end);

//     EXPECT_TRUE(isSorted(vec));
// }


// TEST_P(SortingParameterizedTestMtFloat, HandlesSingleElement) {
//     std::vector<float> vec = {42.2};
//     auto sort = GetParam().func;
    
//     auto start = std::chrono::high_resolution_clock::now();
//     sort(vec);
//     auto end = std::chrono::high_resolution_clock::now();

//     logTiming(GetParam().name + " (HandlesSingleElement)", start, end);

//     EXPECT_TRUE(isSorted(vec));
// }

// TEST_P(SortingParameterizedTestMtFloat, HandlesAlreadySorted) {
//     std::vector<float> vec = {1.2, 2.1, 3.4, 4.7, 5.9};
//     auto sort = GetParam().func;
    
//     auto start = std::chrono::high_resolution_clock::now();
//     sort(vec);
//     auto end = std::chrono::high_resolution_clock::now();

//     logTiming(GetParam().name + " (HandlesAlreadySorted)", start, end);

//     EXPECT_TRUE(isSorted(vec));
// }

// TEST_P(SortingParameterizedTestMtFloat, HandlesReverseSorted) {
//     std::vector<float> vec = {9.34f, 7.1f, 5.044f, 3.3f, 1.201f};
//     auto sort = GetParam().func;
    
//     auto start = std::chrono::high_resolution_clock::now();
//     sort(vec);
//     auto end = std::chrono::high_resolution_clock::now();

//     logTiming(GetParam().name + " (HandlesReverseSorted)", start, end);

//     EXPECT_TRUE(isSorted(vec));
// }

// ---- Instantiation ---- //

// INSTANTIATE_TEST_SUITE_P(
//     SortImplementations,
//     SortingParameterizedTestMtFloat,
//     ::testing::Values(
//         NamedSortFloat{"QuickSortMt", algolab::sort_mt::quickSortAll<float>}
//     ),
//     NameFromStruct<NamedSortFloat>
// );