#pragma once

#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <memory>
#include <iterator>
#include <algorithm> // For std::copy
#include <numeric>   // For std::accumulate

namespace algolab {

// class template for Vector using Value Storage
template <class T>
class Vector {
private:
    T* ptr_;
    uint32_t capacity_;
    uint32_t num_elements_;
    uint8_t capacityMethod_;

public:
    static constexpr uint32_t DEFAULT_CAPACITY = 1741;

    static constexpr uint8_t DEFAULT_CAPACITY_METHOD = 1; // Double
    static constexpr uint8_t LOG_CAPACITY_METHOD = 2; // Log

    constexpr explicit Vector(const uint32_t capacity = DEFAULT_CAPACITY, const uint8_t capacityMethod = DEFAULT_CAPACITY_METHOD) 
        : ptr_(new T[capacity]), capacity_(capacity), num_elements_(0), capacityMethod_(capacityMethod) {
    }

    virtual ~Vector() {
        if (ptr_ != nullptr) {
            delete [] ptr_;
            ptr_ = nullptr;
        }
    }

    Vector(const Vector<T>& other) 
        : ptr_(new T[other.capacity_]), capacity_(other.capacity_), num_elements_(other.num_elements_), capacityMethod_(other.capacityMethod_) {
        
        std::copy(other.begin(), other.end(), begin());
    }
    
    Vector(Vector<T>&& other) noexcept 
        : ptr_(other.ptr_), capacity_(other.capacity_), num_elements_(other.num_elements_), capacityMethod_(other.capacityMethod_) {
    
        other.ptr_ = nullptr;
        other.num_elements_ = 0;
        other.capacity_ = 0;
    }

    Vector<T>& operator=(const Vector<T>& other) {
        if (this != &other) {
            delete[] ptr_;
            ptr_ = new T[other.capacity_];
            num_elements_ = other.num_elements_;
            capacity_ = other.capacity_;
            capacityMethod_ = other.capacityMethod_;
            std::copy(other.begin(), other.end(), begin());
        }
        return *this;
    }

    Vector<T>& operator=(Vector<T>&& other) noexcept {
        if (this != &other) {
            delete [] ptr_;
            ptr_ = other.ptr_;
            num_elements_ = other.num_elements_;
            capacity_ = other.capacity_;
            capacityMethod_ = other.capacityMethod_;
            other.ptr_ = nullptr;
            other.num_elements_ = 0;
            other.capacity_ = 0;
        }
        return *this;
    }

    // Access without bounds checking
    T& operator[](size_t index) {
        return ptr_[index];
    }

    const T& operator[](size_t index) const {
        return ptr_[index];
    }

    uint32_t size() const {
        return  num_elements_;
    }

    uint32_t capacity() const {
        return  capacity_;
    }

    bool empty() const {
        return num_elements_ == 0;
    }

    T& back() const {
        if (empty()) throw std::out_of_range("Vector is empty");
        return ptr_[num_elements_ - 1];
    }

    T& front() const {
        if (empty()) throw std::out_of_range("Vector is empty");
        return ptr_[0];
    }

    void push_back(const T& key) {
        if (num_elements_ >= capacity_) {
            resize();
        }
        ptr_[num_elements_++] = key;
    }

    void push_back(T&& value) {
        if (num_elements_ >= capacity_) {
            resize();
        }
        ptr_[num_elements_++] = std::move(value);
    }

    template<typename... Args>
    void emplace_back(Args&&... args) {
        if (num_elements_ >= capacity_) {
            resize();
        }
        new (&ptr_[num_elements_++]) T(std::forward<Args>(args)...);
    }

    void pop_back() {
        if (num_elements_ > 0) {
            --num_elements_;
        }
    }

    T& at(const uint32_t index) {
        if (index >= num_elements_) {
            throw std::out_of_range("Index out of range");
        }
        return ptr_[index];
    }

    void erase(uint32_t index) {
        if (index >= num_elements_) {
            throw std::out_of_range("Index out of range");
        }
        for (uint32_t i = index; i < num_elements_ - 1; ++i) {
            ptr_[i] = ptr_[i + 1];
        }
        --num_elements_;
    }

    void clear() {
        num_elements_ = 0;
    }

    void swap(Vector<T>& other) noexcept {
        std::swap(ptr_, other.ptr_);
        std::swap(num_elements_, other.num_elements_);
        std::swap(capacity_, other.capacity_);
        std::swap(capacityMethod_, other.capacityMethod_);
    }
    
    void reserve(uint32_t newCapacity) {
        if (newCapacity <= capacity_) return;
        reallocate(newCapacity);
    }

    void reallocate(uint32_t newCapacity) {
        T* newData = new T[newCapacity];
        for (size_t i = 0; i < num_elements_; ++i) {
            newData[i] = std::move(ptr_[i]);
        }
        delete [] ptr_;
        ptr_ = newData;
        capacity_ = newCapacity;
    }

    void shrink_to_fit() {
        if (num_elements_ >= capacity_) return;
        reallocate(num_elements_);
    }

    // Compute the average of elements
    double average() const {
        if (num_elements_ == 0) {
            throw std::runtime_error("Cannot compute average of an empty vector");
        }
        double sum = std::accumulate(begin(), end(), 0.0);
        return sum / num_elements_;
    }

    // Compute the median of elements
    double median() const {
        if (num_elements_ == 0) {
            throw std::runtime_error("Cannot compute median of an empty vector");
        }
        std::vector<T> sorted(begin(), end());
        std::sort(sorted.begin(), sorted.end());

        size_t middle = num_elements_ / 2;
        if (num_elements_ % 2 == 0) {
            return (sorted[middle - 1] + sorted[middle]) / 2.0;
        } else {
            return sorted[middle];
        }
    }

    // Iterators
    T* begin() { return ptr_; }
    T* end() { return ptr_ + num_elements_; }

    const T* begin() const { return ptr_; }  // Const version
    const T* end() const { return ptr_ + num_elements_; }  // Const version

    size_t memory_usage_bytes() const {
        return sizeof(*this) + sizeof(T) * capacity_;
    }    

private:
    // Resize based on capacity method
    void resize() {
        uint32_t newCapacity;
        if (capacityMethod_ == LOG_CAPACITY_METHOD) {
            newCapacity = capacity_ + static_cast<uint32_t>(std::log2(capacity_));
        } else {
            newCapacity = capacity_ == 0 ? 1 : capacity_ * 2;
        }
        if (capacity_ > UINT32_MAX / 2) {
            throw std::overflow_error("Exceeded max vector capacity");
        }
        reserve(newCapacity);
    }
};

} // namespace algolab
