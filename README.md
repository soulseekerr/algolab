# 🧪 algolab

A modern C++ playground to implement, test, and benchmark classic sorting algorithms.  
Designed for performance experiments, algorithm comparison, and clean test-driven development.
Includes reusable test infrastructure, randomized input generation, and Google Test integration.


![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)
![Tested with Google Test](https://img.shields.io/badge/tested-Google%20Test-success)

---

## 📦 Features

- 🧠 **Algorithms Included**
  - Bubble Sort
  - Selection Sort
  - Merge Sort
  - Quick Sort
  - Intro Sort (hybrid algorithm to reflect std::sort performance using median of three partitioning)
  - `std::sort` (baseline)

Intro Sort 
 - Hybrid algorithm to reflect std::sort performance
 - Optimal for performance and stability in real-world data.
 - Improves pivot quality and reduces the risk of worst-case behavior.
 - use of median of three partitioning:
  - picks the median of first, middle, and last elements as pivot.
  - great at avoiding bad pivot choices on already sorted / reversed inputs.
  - often used in std::sort, especially combined with Introsort.


- 🔍 **Extensive Test Coverage**
  - Empty vectors
  - Pre-sorted and reverse-sorted input
  - Duplicate elements
  - Randomized data (int & float)
  - Single-element edge case

- ⚙️ **Parameterized Tests**
  - Shared test logic across algorithms
  - Each sorting algorithm is tested with a shared fixture, using NamedSort structs for readable output:
    ```
    [ RUN      ] SortImplementations/BubbleSort.SortsSampleCorrectly
    [ RUN      ] SortImplementations/QuickSort.HandlesRandomCorrectly
    ```


- 🧪 **Benchmarks Ready**
  - Optional `DISABLED_` benchmark tests for performance measurement
  - Clean microbenchmark format using `<chrono>`

---

## 🛠 Project Structure

algolab/
├── include/
│   └── sort.h    # All algorithm declarations
├── src/
│   └── sort.cpp  # Implementations of each algorithm
├── tests/
|   └── main.cpp
│   └── bubble_sort_test.cpp            # Bubble Sort with Google Test
│   └── merge_sort_test.cpp             # Merge Sort with Google Test
│   └── parameterized_sort_test.cpp     # Parameterized test suite with Google Test
├── CMakeLists.txt


📋 Example: Benchmark Output (SortsRandomCorrectly 50,000 integers)

BubbleSort took 10.1225 s!
SelectionSort took 3.32788 s
MergeSort took 32.5168 ms
HeapSort took 12.3055 ms
QuickSort took 8.08758 ms
QuickSort Iterative took 8.05108 ms
IntroSort took 5.90246 ms
StdSort took 2.65817 ms

Performance Summary

Algorithm	Time	Notes
std::sort	2.65 ms	✅ Compiler-tuned introsort (with insertion + heap fallback)
IntroSort	5.90 ms	🔥 Very close to std::sort, great result!
QuickSort	8.08 ms	Solid baseline recursive quicksort
QuickSort Iterative	8.05 ms	Equivalent performance, more stack-friendly
HeapSort	12.30 ms	Good for worst-case, but slower due to heap overhead
MergeSort	32.51 ms	Stable but heavier memory usage and cache penalties
SelectionSort	3.33 s	🐌 O(n²), expected
BubbleSort	10.12 s	🐢 Brutal! Educational only


🔍 Observations
IntroSort is just ~2× slower than std::sort.
Recursive and iterative quicksort perform almost identically (as they should).
std::sort remains fastest, thanks to compiler optimizations, CPU branch prediction, and inlining.
MergeSort is slower due to heap allocations and poor cache locality, even though it’s O(n log n).
HeapSort is great in fallback scenarios.


🚀 Getting Started

git clone https://github.com/soulseekerr/algolab.git
cd algolab
mkdir build && cd build
cmake ..
make
./debug/Debug/algolabtests


🧑‍💻 Requirements

C++20
CMake 3.16+
Google Test


📚 Future Ideas

Add benchmark variance or median across N runs.
Add test with partially sorted arrays, or with duplicate-heavy arrays.
Test std::stable_sort (for merge-based sorting with stability).
Memory usage benchmarks
Sorting on custom structs
