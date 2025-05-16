#include <gtest/gtest.h>
#include "benchmark_logger.h"

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();

    // After all tests complete:
    BenchmarkLogger::get().prettyPrint();
    // BenchmarkLogger::get().writeCSV("sort_benchmarks.csv");

    return result;
}
