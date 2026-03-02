#ifndef SORT_LOGIC_H
#define SORT_LOGIC_H

#include <string>
#include "my_vector.h"

struct Entry {
    char plate[9]; // "A 000 AA" + '\0'
    std::string value;
};

// Функция для получения значения разряда: 
// step 0: последняя буква, 1: предпоследняя, 2: число 0-999, 3: первая буква
int get_digit(const Entry& e, int step);

// Сама сортировка (использует оптимизацию Base 1000)
void radix_sort(MyVector<Entry>& vec);

#endif