#include "sort_logic.h"

int get_digit(const Entry& e, int step) {
    switch (step) {
        case 0: return e.plate[7] - 'A';      // Вторая буква в конце
        case 1: return e.plate[6] - 'A';      // Первая буква в конце
        case 2: return (e.plate[2]-'0')*100 + (e.plate[3]-'0')*10 + (e.plate[4]-'0');
        case 3: return e.plate[0] - 'A';      // Самая первая буква
        default: return 0;
    }
}

void radix_sort(MyVector<Entry>& vec) {
    size_t n = vec.size();
    if (n < 2) return;

    // Вспомогательный массив (буфер) для сортировки
    Entry* temp = new Entry[n];
    Entry* src = &vec[0];
    Entry* dst = temp;

    for (int step = 0; step < 4; ++step) {
        int range = (step == 2) ? 1000 : 26;
        int count[1000] = {0};

        for (size_t i = 0; i < n; ++i) count[get_digit(src[i], step)]++;
        for (int i = 1; i < range; ++i) count[i] += count[i - 1];
        
        // Идем с конца для устойчивости (stability)
        for (int i = (int)n - 1; i >= 0; --i) {
            int d = get_digit(src[i], step);
            dst[--count[d]] = std::move(src[i]);
        }
        // Меняем местами указатели на буферы
        std::swap(src, dst);
    }

    // Если данные застряли в temp (src указывает на него), переносим в основной вектор
    if (src == temp) {
        for (size_t i = 0; i < n; ++i) vec[i] = std::move(temp[i]);
    }
    
    delete[] temp;
}