#include <iostream>
#include "include/sort_logic.h"

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    MyVector<Pair> data;
    char p1;
    int num;
    std::string p_last;
    
    while (std::cin >> p1 >> num >> p_last) {
        Pair p;
        
        p.plate[0] = p1;
        p.plate[1] = ' ';
        p.plate[2] = (num / 100) + '0';
        p.plate[3] = ((num / 10) % 10) + '0';
        p.plate[4] = (num % 10) + '0';
        p.plate[5] = ' ';
        p.plate[6] = p_last[0];
        p.plate[7] = p_last[1];
        p.plate[8] = '\0';
        
        std::string val;
        std::getline(std::cin, val);
        if (!val.empty() && val[0] == '\t') {
            p.value = val.substr(1);
        } else {
            p.value = val;
        }

        data.push_back(std::move(p));
    }

    radix_sort(data);

    for (size_t i = 0; i < data.size(); ++i) {
        std::cout << data[i].plate << "\t" << data[i].value << "\n";
    }

    return 0;
}