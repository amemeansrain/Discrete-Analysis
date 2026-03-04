#include <iostream>
#include <chrono>
#include <algorithm>
#include <string>
#include <iomanip>
#include <cstring>

#include "sort_logic.h"
#include "my_vector.h"

using duration_t = std::chrono::microseconds;
const std::string DURATION_PREFIX = "us";

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);

    MyVector<Pair> input;
    std::string line;

    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;

        size_t tab_pos = line.find('\t');
        if (tab_pos != std::string::npos) {
            Pair e;
            std::string plate_str = line.substr(0, tab_pos);
            
            size_t len = plate_str.copy(e.plate, 8);
            e.plate[len] = '\0';
            
            e.value = line.substr(tab_pos + 1);
            input.push_back(std::move(e));
        }
    }

    if (input.size() == 0) {
        std::cout << "No info for benchmark." << std::endl;
        return 0;
    }

    MyVector<Pair> input_stl;
    for(size_t i = 0; i < input.size(); ++i) {
        Pair e;
        std::memcpy(e.plate, input[i].plate, 9);
        e.value = input[i].value;
        input_stl.push_back(std::move(e));
    }

    std::cout << "Amount of lines: " << input.size() << "\n";
    std::cout << "-----------------------------------\n";

    auto start_ts = std::chrono::high_resolution_clock::now();
    radix_sort(input);
    auto end_ts = std::chrono::high_resolution_clock::now();
    
    uint64_t radix_sort_ts = std::chrono::duration_cast<duration_t>(end_ts - start_ts).count();
    std::cout << "My Radix Sort:  " << radix_sort_ts << " " << DURATION_PREFIX << "\n";

    auto start_stl = std::chrono::high_resolution_clock::now();
    
    std::stable_sort(&input_stl[0], &input_stl[0] + input_stl.size(), 
        [](const Pair& a, const Pair& b) {
            return std::string_view(a.plate) < std::string_view(b.plate);
        });
        
    auto end_stl = std::chrono::high_resolution_clock::now();

    uint64_t stl_sort_ts = std::chrono::duration_cast<duration_t>(end_stl - start_stl).count();
    std::cout << "Stable Sort:    " << stl_sort_ts << " " << DURATION_PREFIX << "\n";
    
    std::cout << "-----------------------------------\n";
    
    if (radix_sort_ts > 0) {
        double speedup = (double)stl_sort_ts / radix_sort_ts;
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Result: My Radix Sort is " << speedup << "x faster than Stable Sort.\n";
    }

    return 0;
}