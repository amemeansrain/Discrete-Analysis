#ifndef MY_VECTOR_H
#define MY_VECTOR_H

template <typename T>
class MyVector {
private:
    T* data;
    size_t sz;
    size_t capacity;

    void reserve(size_t new_capacity) {
        if (new_capacity <= capacity) return;
        T* new_data = new T[new_capacity];
        for (size_t i = 0; i < sz; ++i) {
            new_data[i] = std::move(data[i]);
        }
        delete[] data;
        data = new_data;
        capacity = new_capacity;
    }

public:
    MyVector() : data(nullptr), sz(0), capacity(0) {}
    MyVector(size_t n) : data(new T[n]), sz(n), capacity(n) {}

    ~MyVector() { delete[] data; }

    MyVector(const MyVector& other) : data(nullptr), sz(0), cap(0) {
        reserve(other.cap);
        for (size_t i = 0; i < other.sz; ++i) {
            data[i] = other.data[i];
        }
        sz = other.sz;
    }

    MyVector& operator=(MyVector other) {
        std::swap(data, other.data);
        std::swap(sz, other.sz);
        std::swap(capacity, other.capacity);
        return *this;
    }

    void push_back(T&& value) {
        if (sz == capacity) reserve(capacity == 0 ? 1 : capacity * 2);
        data[sz++] = std::move(value);
    }

    T& operator[](size_t i) { return data[i]; }
    const T& operator[](size_t i) const { return data[i]; }
    size_t size() const { return sz; }
};

#endif