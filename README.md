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
  - `std::sort` (baseline)

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
│   └── sort.h              # All algorithm declarations
├── src/
│   └── sort.cpp            # Implementations of each algorithm
├── tests/
│   └── bubble_sort_test.cpp            # Bubble Sort with Google Test
│   └── merge_sort_test.cpp             # Merge Sort with Google Test
│   └── parameterized_sort_test.cpp     # Parameterized test suite with Google Test
├── CMakeLists.txt


📋 Example: Benchmark Output (100,000 integers)

BubbleSort took 9123.5 ms
SelectionSort took 5342.8 ms
MergeSort took 12.6 ms
QuickSort took 9.4 ms
StdSort took 7.1 ms


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

Heap Sort, Insertion Sort, Shell Sort
Stability check tests
Parallelized sort variants
Memory usage benchmarks
Sorting on custom structs
Search algorithms (binary, linear, interpolation)
