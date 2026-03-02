#ifndef MY_VECTOR_H
#define MY_VECTOR_H

#include <utility>

template <typename T>
class MyVector {
private:
    T* data;
    size_t sz;
    size_t cap;

    void reserve(size_t new_cap) {
        T* new_data = new T[new_cap];
        for (size_t i = 0; i < sz; ++i) {
            new_data[i] = std::move(data[i]);
        }
        delete[] data;
        data = new_data;
        cap = new_cap;
    }

public:
    MyVector() : data(nullptr), sz(0), cap(0) {}
    ~MyVector() { delete[] data; }

    MyVector(const MyVector&) = delete;
    MyVector& operator=(const MyVector&) = delete;

    void push_back(T&& value) {
        if (sz == cap) reserve(cap == 0 ? 8 : cap * 2);
        data[sz++] = std::move(value);
    }

    T& operator[](size_t i) { return data[i]; }
    const T& operator[](size_t i) const { return data[i]; }
    size_t size() const { return sz; }
};

#endif