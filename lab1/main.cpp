#include <iostream>
#include "include/sort_logic.h"

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