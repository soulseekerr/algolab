
#include <gtest/gtest.h>
#include "hashset.h"

#include <iostream>
#include <unordered_set>
#include <memory>
#include <chrono>
#include <vector>
#include <algorithm>
#include <random>


struct MarketQuote {
    uint64_t id;
    double price;

    MarketQuote(uint64_t i, double p) : id(i), price(p) {}

    bool operator==(const MarketQuote& other) const {
        return id == other.id && price == other.price;
    }
};

struct MarketQuoteHash {
    std::size_t operator()(const MarketQuote& data) const {
        return std::hash<uint64_t>{}(data.id);
    }
};

class HashSetTest : public ::testing::Test {
public:
    algolab::HashSet<int> hashsetInt{};
    algolab::HashSet<uint64_t> hashsetUInt64{};
    algolab::HashSet<std::string, std::hash<std::string>> hashsetStr{};
    std::vector<MarketQuote> marketQuotes{};

    void SetUp() override {
        hashsetInt.insert(10);
        hashsetInt.insert(65);
        hashsetInt.insert(228);
        hashsetInt.insert(561);
        hashsetInt.insert(4958);

        hashsetUInt64.insert(1436596);
        hashsetUInt64.insert(73875350870UL);
        hashsetUInt64.insert(9707435087ULL);
        hashsetUInt64.insert(430847508);
        hashsetUInt64.insert(6586589);
        hashsetUInt64.insert(1436536596);
        hashsetUInt64.insert(675930);

        hashsetStr.insert("hello");
        hashsetStr.insert("world");
        hashsetStr.insert("vince");
        hashsetStr.insert("rafael");
        hashsetStr.insert("anna");

        generateMarketQuotes(1000000, 0.0f, 1000000.0f);
    }

    void generateMarketQuotes(size_t count, float min, float max) {
        marketQuotes.clear();
        std::mt19937_64 rng{std::random_device{}()};
        std::uniform_real_distribution<double> dist(min, max);
        for (uint64_t i = 0; i < count; ++i) {
            marketQuotes.emplace_back(i, dist(rng));
        }
    }

    inline auto now() {
        return std::chrono::high_resolution_clock::now();
    }
};

TEST_F(HashSetTest, InsertAndSearch) {
    EXPECT_TRUE(hashsetInt.search(228));
    EXPECT_TRUE(hashsetInt.search(4958));
    EXPECT_FALSE(hashsetInt.search(12));
    EXPECT_FALSE(hashsetInt.search(66));
    EXPECT_TRUE(hashsetStr.search("vince"));
    EXPECT_FALSE(hashsetStr.search("Raf"));
    EXPECT_TRUE(hashsetStr.search("anna"));
}

TEST_F(HashSetTest, DuplicateInsert) {
    EXPECT_FALSE(hashsetInt.insert(10));
    EXPECT_FALSE(hashsetInt.insert(561));
}

TEST_F(HashSetTest, RemoveElement) {
    EXPECT_TRUE(hashsetInt.remove(10));
    EXPECT_FALSE(hashsetInt.search(10));
    EXPECT_FALSE(hashsetInt.remove(20));
}

TEST_F(HashSetTest, CollisionHandling) {
    algolab::HashSet<int> collisionSet;
    int v1 = 5;
    int v2 = 5 + 11;

    EXPECT_TRUE(collisionSet.insert(v1));
    EXPECT_TRUE(collisionSet.insert(v2));
    EXPECT_TRUE(collisionSet.search(v1));
    EXPECT_TRUE(collisionSet.search(v2));
    EXPECT_TRUE(collisionSet.remove(v1));
    EXPECT_FALSE(collisionSet.search(v1));
    EXPECT_TRUE(collisionSet.search(v2));
}

TEST_F(HashSetTest, ResizeOnLoadFactor) {
    algolab::HashSet<int> resizableSet(0.5);
    for (int i = 0; i < 20; i++) {
        resizableSet.insert(i);
    }

    for (int i = 0; i < 20; i++) {
        EXPECT_TRUE(resizableSet.search(i));
    }
}

TEST_F(HashSetTest, InsertAfterResize) {
    algolab::HashSet<int> resizableSet(0.5);
    for (int i = 0; i < 25; i++) {
        resizableSet.insert(i);
    }

    EXPECT_TRUE(resizableSet.insert(50));
    EXPECT_TRUE(resizableSet.search(50));
}

TEST_F(HashSetTest, MarketQuotesTest1) {   
    std::unordered_set<MarketQuote, MarketQuoteHash> stlSet;

    ASSERT_EQ(marketQuotes.size(), 1'000'000);

    auto total_start = now();
    auto start = now();

    for (const auto& item : marketQuotes) {
        stlSet.insert(item);
    }
    auto end = now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "STL unordered_set time [insert]: " << duration.count() << " seconds" << std::endl;

    start = now();
    for (const auto& item : marketQuotes) {
        stlSet.find(item);
    }
    end = now();
    duration = end - start;
    std::cout << "STL unordered_set time [search]: " << duration.count() << " seconds" << std::endl;

    start = now();
    for (const auto& item : marketQuotes) {
        stlSet.erase(item);
    }
    end = now();
    duration = end - start;
    std::cout << "STL unordered_set time [remove]: " << duration.count() << " seconds" << std::endl;

    auto total_end = now();
    std::chrono::duration<double> total = total_end - total_start;
    std::cout << "STL unordered_set time: " << total.count() << " seconds" << std::endl;
}

TEST_F(HashSetTest, MarketQuotesTest2) {  
    auto total_start = now();

    ASSERT_EQ(marketQuotes.size(), 1'000'000);

    auto start = now();
    algolab::HashSet<MarketQuote, MarketQuoteHash> customSet;
    for (const auto& item : marketQuotes) {
        customSet.insert(item);
    }
    auto end = now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Custom HashSet time [insert]: " << duration.count() << " seconds" << std::endl;

    start = now();
    for (const auto& item : marketQuotes) {
        customSet.search(item);
    }
    end = now();
    duration = end - start;
    std::cout << "Custom HashSet time [search]: " << duration.count() << " seconds" << std::endl;

    start = now();
    for (const auto& item : marketQuotes) {
        customSet.remove(item);
    }
    end = now();
    duration = end - start;
    std::cout << "Custom HashSet time [remove]: " << duration.count() << " seconds" << std::endl;

    auto total_end = now();
    duration = total_end - total_start;
    std::cout << "Custom HashSet time: " << duration.count() << " seconds" << std::endl;
}