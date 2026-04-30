#include <iostream>
#include <chrono>
#include <algorithm>
#include <string>
#include <iomanip>
#include <vector>
#include <random>

// Предполагаем, что sort_logic.h и my_vector.h подключены
#include "../include/sort_logic.h"
#include "../include/my_vector.h"

using duration_t = std::chrono::microseconds;

// Функция для генерации случайного номера
Pair generate_random_pair() {
    static const char letters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    static std::mt19937 gen(42); 
    std::uniform_int_distribution<> letter_dist(0, 25);
    std::uniform_int_distribution<> digit_dist(0, 999);

    Pair p;
    p.plate[0] = letters[letter_dist(gen)];
    p.plate[1] = ' ';
    int num = digit_dist(gen);
    p.plate[2] = (num / 100) + '0';
    p.plate[3] = ((num / 10) % 10) + '0';
    p.plate[4] = (num % 10) + '0';
    p.plate[5] = ' ';
    p.plate[6] = letters[letter_dist(gen)];
    p.plate[7] = letters[letter_dist(gen)];
    p.plate[8] = '\0';
    p.value = "some_value";
    return p;
}

void run_test(size_t N) {
    MyVector<Pair> data_radix;
    MyVector<Pair> data_stable;

    // Генерируем данные
    for (size_t i = 0; i < N; ++i) {
        Pair p = generate_random_pair();
        data_radix.push_back(std::move(p));
    }
    
    // Копируем для честного сравнения со стабильной сортировкой
    for (size_t i = 0; i < N; ++i) {
        Pair p;
        std::memcpy(p.plate, data_radix[i].plate, 9);
        p.value = data_radix[i].value;
        data_stable.push_back(std::move(p));
    }

    // Замер Radix Sort
    auto s1 = std::chrono::high_resolution_clock::now();
    radix_sort(data_radix);
    auto e1 = std::chrono::high_resolution_clock::now();
    auto t1 = std::chrono::duration_cast<duration_t>(e1 - s1).count();

    // Замер Stable Sort
    auto s2 = std::chrono::high_resolution_clock::now();
    std::stable_sort(&data_stable[0], &data_stable[0] + data_stable.size(), 
        [](const Pair& a, const Pair& b) {
            return std::string_view(a.plate) < std::string_view(b.plate);
        });
    auto e2 = std::chrono::high_resolution_clock::now();
    auto t2 = std::chrono::duration_cast<duration_t>(e2 - s2).count();

    // Вывод строки таблицы
    std::cout << std::setw(10) << N << " | "
              << std::setw(12) << t1 << " | "
              << std::setw(12) << t2 << " | "
              << std::fixed << std::setprecision(5) << (double)t1 / N << " | "
              << std::setprecision(2) << (double)t2 / t1 << "x\n";
}

int main() {
    std::cout << "Benchmarking Radix Sort Complexity\n";
    std::cout << "---------------------------------------------------------------------------\n";
    std::cout << std::setw(10) << "N" << " | "
              << std::setw(12) << "Radix(us)" << " | "
              << std::setw(12) << "Stable(us)" << " | "
              << std::setw(12) << "Time/N (R)" << " | "
              << "Speedup\n";
    std::cout << "---------------------------------------------------------------------------\n";

    // Увеличиваем N в 2 раза на каждом шаге
    size_t sizes[] = {100000, 200000, 400000, 800000, 1600000};
    for (size_t n : sizes) {
        run_test(n);
    }
    std::cout << "---------------------------------------------------------------------------\n";

    return 0;
}