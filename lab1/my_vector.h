#ifndef MY_VECTOR_H
#define MY_VECTOR_H

#include <utility>

template <typename T>
class MyVector {
private:
    T* data_;
    size_t size_;
    size_t capacity_;

    void reserve_(size_t new_capacity) {
        T* new_data = new T[new_capacity];
        for (size_t i = 0; i < size_; ++i) {
            new_data[i] = std::move(data_[i]);
        }
        delete[] data_;
        data_ = new_data;
        capacity_ = new_capacity;
    }

public:
    MyVector() : data_(nullptr), size_(0), capacity_(0) {}
    ~MyVector() { delete[] data_; }

    MyVector(const MyVector&) = delete;
    MyVector& operator=(const MyVector&) = delete;

    void push_back(T&& value) {
        if (size_ == capacity_) reserve_(capacity_ == 0 ? 8 : capacity_ * 2);
        data_[size_++] = std::move(value);
    }

    T& operator[](size_t i) { return data_[i]; }
    const T& operator[](size_t i) const { return data_[i]; }
    size_t size_() const { return size_; }
};

#endif