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

Insertion Sort, Shell Sort
Stability check tests
Parallelized sort variants
Memory usage benchmarks
Sorting on custom structs
Search algorithms (binary, linear, interpolation)
