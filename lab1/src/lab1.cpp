#include <iostream>
#include <string>

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

struct Pair {
    char plate[9];
    std::string value;
};

int get_digit(const Pair& e, int step) {
    switch (step) {
        case 0: return e.plate[7] - 'A';
        case 1: return e.plate[6] - 'A';
        case 2: return (e.plate[2] - '0') * 100 + (e.plate[3] - '0') * 10 + (e.plate[4] - '0');
        case 3: return e.plate[0] - 'A';
    }
}

void radix_sort(MyVector<Pair>& vec) {
    size_t n = vec.size();
    if (n < 2) return;

    Pair* temp = new Pair[n];
    Pair* original = &vec[0];
    Pair* result = temp;

    for (int step = 0; step < 4; ++step) {
        int range = (step == 2) ? 1000 : 26;
        int count[1000] = {0};

        for (size_t i = 0; i < n; ++i) count[get_digit(original[i], step)]++;
        for (int i = 1; i < range; ++i) count[i] += count[i - 1];
        
        for (int i = (int)n - 1; i >= 0; --i) {
            int d = get_digit(original[i], step);
            result[--count[d]] = std::move(original[i]);
        }
        
        std::swap(original, result);
    }
    delete[] temp;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    MyVector<Pair> data;
    char p1, p2, p3;
    int num;
    
    while (std::cin >> p1 >> num >> p2 >> p3) {
        Pair e;
        
        std::string num_str = std::to_string(num);
        while (num_str.length() < 3) num_str = "0" + num_str;
        e.key = p1 + " " + num_str + " " + p2;
        
        std::string val;
        std::getline(std::cin, val);
        if (!val.empty() && val[0] == '\t') e.value = val.substr(1);
        else e.value = val;

        data.push_back(std::move(e));
    }

    radix_sort(data);

    for (size_t i = 0; i < data.size(); ++i) {
        std::cout << data[i].plate << "\t" << data[i].value << "\n";
    }

    return 0;
}