#ifndef SORT_LOGIC_H
#define SORT_LOGIC_H

#include <string>
#include "my_vector.h"

struct Pair {
    char plate[9];
    std::string value;
};

int get_digit(const Pair& p, int step);

void radix_sort(MyVector<Pair>& vec);

#endif