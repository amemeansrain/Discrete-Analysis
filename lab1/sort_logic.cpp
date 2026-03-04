#include "sort_logic.h"

int get_digit(const Entry& e, int step) {
    switch (step) {
        case 0: return e.plate[7] - 'A';
        case 1: return e.plate[6] - 'A';
        case 2: return (e.plate[2] - '0') * 100 + (e.plate[3] - '0') * 10 + (e.plate[4] - '0');
        case 3: return e.plate[0] - 'A';
    }
}

void radix_sort(MyVector<Entry>& vec) {
    size_t n = vec.size();
    if (n < 2) return;

    Entry* temp = new Entry[n];
    Entry* original = &vec[0];
    Entry* result = temp;

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