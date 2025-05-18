
#include <gtest/gtest.h>
#include <vector>
#include "vector.h"
#include <thread>
#include <mutex>
#include <shared_mutex>

struct Person {
    std::string name;
    int age;

    // For further use below, add default ctor
    Person() : name(""), age(0) {}

    Person(std::string n, int a) : name(std::move(n)), age(a) {}
    bool operator==(const Person& other) const {
        return name == other.name && age == other.age;
    }
};

TEST(VectorTest, PushBackAndAccess) {
    algolab::Vector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    
    EXPECT_EQ(vec.size(), 3);
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[2], 3);
    EXPECT_FALSE(vec.empty());
    EXPECT_EQ(vec.front(), 1);
    EXPECT_EQ(vec.back(), 3);
}

TEST(VectorTest, CopyAndMoveSemantics) {
    algolab::Vector<std::string> a;
    a.push_back("hello");

    algolab::Vector<std::string> b = a; // copy
    EXPECT_EQ(b[0], "hello");

    algolab::Vector<std::string> c = std::move(a); // move
    EXPECT_EQ(c[0], "hello");
}

TEST(VectorTest, ResizeAndReserve) {
    algolab::Vector<int> v(2); // small capacity
    for (int i = 0; i < 10; ++i) v.push_back(i);

    EXPECT_EQ(v.size(), 10);
    EXPECT_GE(v.capacity(), 10);
    
    v.shrink_to_fit();
    EXPECT_EQ(v.capacity(), 10);
}

TEST(VectorTest, AverageAndMedian) {
    algolab::Vector<double> v;
    v.push_back(1.0);
    v.push_back(2.0);
    v.push_back(3.0);

    EXPECT_DOUBLE_EQ(v.average(), 2.0);
    EXPECT_DOUBLE_EQ(v.median(), 2.0);

    v.push_back(4.0);
    EXPECT_DOUBLE_EQ(v.median(), 2.5);
}

TEST(VectorTest, EmplaceBackWithPerson) {
    algolab::Vector<Person> people;
    
    // Test emplace_back
    people.emplace_back("Alice", 30);
    people.emplace_back("Bob", 45);

    EXPECT_EQ(people.size(), 2);
    EXPECT_EQ(people[0].name, "Alice");
    EXPECT_EQ(people[1].age, 45);

    // Test push_back with move
    Person p("Charlie", 50);
    people.push_back(std::move(p));

    EXPECT_EQ(people.size(), 3);
    EXPECT_EQ(people[2].name, "Charlie");
}

TEST(VectorBenchmark, CompareWithStdVector) {
    constexpr size_t N = 1'000'000;

    std::vector<int> stdVec;
    algolab::Vector<int> customVec;

    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < N; ++i) stdVec.push_back(i);
    auto end = std::chrono::high_resolution_clock::now();
    auto stdDuration = std::chrono::duration<double>(end - start).count();

    start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < N; ++i) customVec.push_back(i);
    end = std::chrono::high_resolution_clock::now();
    auto customDuration = std::chrono::duration<double>(end - start).count();

    std::cout << "std::vector time: " << stdDuration << "s\n";
    std::cout << "algolab::Vector time: " << customDuration << "s\n";

    EXPECT_EQ(customVec.size(), stdVec.size());
    EXPECT_EQ(customVec.front(), stdVec.front());
    EXPECT_EQ(customVec.back(), stdVec.back());
}

TEST(MultithreadedVectorTest, ConcurrentPushBack) {
    algolab::Vector<int> vec;
    std::shared_mutex vec_mutex;
    constexpr int thread_count = 8;
    constexpr int per_thread = 10000;

    std::vector<std::jthread> threads;

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < thread_count; ++i) {
        threads.emplace_back([&vec, &vec_mutex, i]() {
            for (int j = 0; j < per_thread; ++j) {
                std::unique_lock lock(vec_mutex);
                vec.push_back(i * per_thread + j);
            }
        });
    }

    for (auto& t : threads) t.join();

    auto end = std::chrono::high_resolution_clock::now();
    auto stdDuration = std::chrono::duration<double>(end - start).count();
    std::cout << "MultithreadedVectorTest time: " << stdDuration << "s\n";

    EXPECT_EQ(vec.size(), thread_count * per_thread);
}

TEST(MemoryFootprintTest, CompareVectorMemoryUsage) {
    constexpr size_t N = 100000;

    auto start_std = std::chrono::high_resolution_clock::now();
    std::vector<int> stdVec;
    for (size_t i = 0; i < N; ++i) {
        stdVec.push_back(i);
    }
    auto end_std = std::chrono::high_resolution_clock::now();
    auto dur_std = std::chrono::duration<double, std::milli>(end_std - start_std).count();

    auto start_custom = std::chrono::high_resolution_clock::now();
    algolab::Vector<int> myVec;
    for (size_t i = 0; i < N; ++i) {
        myVec.push_back(i);
    }
    auto end_custom = std::chrono::high_resolution_clock::now();
    auto dur_custom = std::chrono::duration<double, std::milli>(end_custom - start_custom).count();

    std::cout << "Memory Footprint Test:\n";
    std::cout << "Vector Int size = " << stdVec.size() << " elements\n";

    std::cout << "std::vector time: " << dur_std << " ms\n";
    std::cout << "algolab::Vector time: " << dur_custom << " ms\n";
    std::cout << "std::vector capacity: " << stdVec.capacity() << ", custom vector capacity: " << myVec.capacity() << "\n";
    std::cout << "std::vector memory: " << sizeof(int) * stdVec.capacity() << " bytes\n";
    std::cout << "algolab::Vector memory: " << sizeof(int) * myVec.capacity() << " bytes\n";

    EXPECT_EQ(stdVec.size(), myVec.size());
}
