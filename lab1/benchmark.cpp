#include <iostream>
#include <chrono>
#include "sort_logic.h"

int main() {
    const size_t N = 1000000;
    MyVector<Entry> test_data;

    auto start = std::chrono::high_resolution_clock::now();
    radix_sort(test_data);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> diff = end - start;
    std::cout << "Sorted " << N << " elements in " << diff.count() << " seconds." << std::endl;
    
    return 0;
}