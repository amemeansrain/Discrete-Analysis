#include <iostream>
#include <cstdio>
#include "sort_logic.h"

int main() {
    // Ускоряем ввод-вывод
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    MyVector<Entry> data;
    char p1, p2, p3;
    int num;
    
    // Считываем по формату "A 000 AA"
    while (std::cin >> p1 >> num >> p2 >> p3) {
        Entry e;
        sprintf(e.plate, "%c %03d %c%c", p1, num, p2, p3);
        
        // Считываем остаток строки (значение после табуляции)
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