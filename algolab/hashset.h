#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <functional>
#include <string>
#include <cstdint>
#include <array>
#include <shared_mutex>
#include <type_traits>
#include "utils.h" // Assuming this is where DEBUG_LOG is defined

namespace algolab {

    /** 
 * @brief Node_t structure
 * @details Node structure for the linked list in each bucket
 * 
 */
template <typename T>
struct Node_t {
    T key;
    std::unique_ptr<Node_t<T>> next;    
    explicit Node_t(const T& k) : key(k), next(nullptr) {}
};

/**
 * @brief ThomasWangHash struct
 * @details Thomas Wang's 64-bit to 32-bit hash function
 * Designed to produce a well-distributed hash value from a 64-bit integer key
 * Good distribution: it is designed to minimize collisions.
 * Efficiency: Uses bitwise operations and multiplications, which are fast on modern CPUs.
 * Deterministic: The same input will always produce the same output.
 */
 struct ThomasWangHash {
    // This allows use of ThomasWangHash for any integral type (int, long, etc.) safely, not just uint64_t.
    template <typename T>
    constexpr uint32_t operator()(T key) const {
        static_assert(std::is_integral<T>::value, "ThomasWangHash requires integral types.");
        // Flips bits (~key) and adds a left shift to introduce randomness
        key = (~key) + (key << 18); // key = (key << 18) - key - 1;
        // Further scrambles by XORing with a right shift
        key = key ^ (key >> 31);
        // Multiplication spreads bits further
        key = key * 21; // key = (key + (key << 2)) + (key << 4);
        // These steps continue to shuffle the bits using XORs and shifts
        key = key ^ (key >> 11);
        key = key + (key << 6);
        key = key ^ (key >> 22);
        // Final cast to 32 bits, extracts the lower 32 bits, discarding the upper half
        return static_cast<uint32_t>(key);
    }
};

/**
 * @brief HashSet_t class
 * @details HashSet class with insert, search, remove and display functions
 * Thread safety is ensured via std::shared_mutex (multiple readers, single writer).
 * static_assert guards against misuse of the ThomasWangHash with non-integral types.
 * size() and capacity() now lock safely using shared access.
 * Write operations (insert, remove, resize, clear) use unique_lock.
 * 
 */
template <typename T, typename Hash = ThomasWangHash, typename KeyEqual = std::equal_to<T> >
class HashSet {
    static_assert(std::is_integral<T>::value, "ThomasWangHash only supports integral keys.");

private:
    uint64_t bucketCount;
    uint64_t elementCount;
    double loadFactor; // Load factor variable

    mutable std::shared_mutex mutex_; // Mutex for thread safety

    // std::unique_ptr<std::unique_ptr<Node_t<T>>[]> buckets;
    std::vector<std::unique_ptr<Node_t<T>>> buckets;

    // Static constexpr default load factor
    static constexpr double defaultLoadFactor = 0.7;

    static constexpr std::array<uint64_t, 33> primeSizes {
        11ULL, 23ULL, 47ULL, 97ULL, 199ULL, 409ULL, 823ULL, 1741ULL, 3469ULL, 6949ULL, 14033ULL,
        28067ULL, 56103ULL, 112213ULL, 224467ULL, 448949ULL, 897919ULL, 1795847ULL,
        3591703ULL, 7183417ULL, 14366889ULL, 28733777ULL, 57467521ULL, 114935069ULL,
        229870171ULL, 459740359ULL, 919480687ULL, 1838961469ULL, 3677922933ULL,
        7355845867ULL, 14711691733ULL, 29423383469ULL, 58846766941ULL
    }; // Example primes

    int currentPrimeIndex;

    Hash hasher;
    KeyEqual keyEqual;

public:
    explicit HashSet(float p_loadFactor = defaultLoadFactor) 
        : elementCount(0), currentPrimeIndex(0) {
        loadFactor = p_loadFactor;
        bucketCount = primeSizes[currentPrimeIndex];
        buckets.resize(bucketCount);
    }

    virtual ~HashSet() = default;

    // Insert Key
    bool insert(const T& key) {
        std::unique_lock lock(mutex_);
    
        if (elementCount > bucketCount * loadFactor) {
            resize();
        }

        const uint64_t hashValue = getHash(key);
        for (Node_t<T>* node = buckets[hashValue].get(); node; node = node->next.get()) {
            if (keyEqual(node->key, key)) return false;
        }

        auto newNode = std::make_unique<Node_t<T>>(key);
        newNode->next = std::move(buckets[hashValue]);
        buckets[hashValue] = std::move(newNode);
        ++elementCount;
        DEBUG_LOG(std::format("Inserted key: {} at bucket: {}", key, hashValue));
        return true;
    }

    // // Search Key
    bool search(const T& key) const {
        std::shared_lock lock(mutex_);

        uint64_t hashValue = getHash(key);
        Node_t<T>* current = buckets[hashValue].get();
        
        while (current != nullptr) {
            if (keyEqual(current->key, key)) {
                DEBUG_LOG(std::format("Search key: {} found at bucket: {}", key, hashValue));
                return true;
            }
            current = current->next.get();
        }
#ifdef DEBUG_CONTAINER
        std::cout << "Key: " << key << " not found" << std::endl;
#endif
        return false;
    }

    // Remove Key
    bool remove(const T& key) {
        std::unique_lock lock(mutex_);

        uint64_t hashValue = getHash(key);
        Node_t<T>* current = buckets[hashValue].get();
        Node_t<T>* prev = nullptr;
    
        while (current != nullptr) {
            if (keyEqual(current->key, key)) {
                if (prev == nullptr) {
                    buckets[hashValue] = std::move(current->next);
                } else {
                    prev->next = std::move(current->next);
                }
                DEBUG_LOG(std::format("Removed key: {} from bucket: {}", key, hashValue));
                --elementCount;
                return true;
            }
            prev = current;
            current = current->next.get();
        }
        DEBUG_LOG(std::format("Key: {} not found for removal. Removal skipped.", key));
        return false;
    }

    void clear() {
        std::unique_lock lock(mutex_);

        for (auto& bucket : buckets) {
            bucket.reset();
        }
        elementCount = 0;
        currentPrimeIndex = 0;
        bucketCount = primeSizes[currentPrimeIndex];
        buckets.resize(bucketCount);
    }

    void display() const {
        std::shared_lock lock(mutex_);

        std::cout << "HashSet contents:" << std::endl;
        for (uint64_t i = 0; i < bucketCount; ++i) {
            std::cout << "Bucket " << i << ": ";
            Node_t<T>* current = buckets[i].get();
            while (current != nullptr) {
                std::cout << current->key << " -> ";
                current = current->next.get();
            }
            std::cout << "nullptr" << std::endl;
        }
    }

    std::size_t size() const { 
        std::shared_lock lock(mutex_);
        return elementCount; 
    }

    std::size_t capacity() const { 
        std::shared_lock lock(mutex_);
        return bucketCount; 
    }

    double load_factor() const {
        std::shared_lock lock(mutex_);

        return static_cast<double>(elementCount) / bucketCount;
    }

    template<typename Callback>
    void forEach(Callback&& cb) const {
        std::shared_lock lock(mutex_);

        for (const auto& bucket : buckets) {
            for (Node_t<T>* node = bucket.get(); node; node = node->next.get()) {
                cb(node->key);
            }
        }
    }

private:
    // Get hash value for a key
    uint64_t getHash(const T& key) const {
        return hasher(key) % bucketCount;
    }

    // Resize function to move to the next prime size
    void resize() {
        if (currentPrimeIndex + 1 >= primeSizes.size()) return; // No more primes available

        auto oldBucketCount = bucketCount;
        bucketCount = primeSizes[++currentPrimeIndex];

        std::vector<std::unique_ptr<Node_t<T>>> newBuckets(bucketCount);

        for (size_t i = 0; i < oldBucketCount; ++i) {
            Node_t<T>* current = buckets[i].get();
            while (current) {
                uint64_t newHashValue = hasher(current->key) % bucketCount;
                auto newNode = std::make_unique<Node_t<T>>(current->key);
                newNode->next = std::move(newBuckets[newHashValue]);
                newBuckets[newHashValue] = std::move(newNode);
                current = current->next.get();
            }
        }
        buckets = std::move(newBuckets);
    }
};

} // namespace algolab
