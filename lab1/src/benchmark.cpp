#include <iostream>
#include <chrono>
#include <random>
#include <cstring>
#include "../include/sort_logic.h"

// Функция генерации данных (ЗАПОЛНЕНИЕ)
void fill_random(MyVector<Pair>& vec, size_t count) {
    const char letters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    
    for (size_t i = 0; i < count; ++i) {
        Pair e;
        
        // Генерируем формат A 999 BC
        char p1 = letters[rand() % 26];
        int num = rand() % 1000;
        char p2 = letters[rand() % 26];
        char p3 = letters[rand() % 26];
        
        // Записываем в массив plate
        snprintf(e.plate, 9, "%c %03d %c%c", p1, num, p2, p3);
        
        // Генерируем случайную строку-значение
        e.value = "Value_" + std::to_string(i); 
        
        // КЛАДЕМ В ВЕКТОР (здесь цикл работает на наполнение)
        vec.push_back(std::move(e));
    }
}

// Та самая функция run_test, про которую мы говорили
void run_test(size_t N) {
    MyVector<Pair> test_data;
    
    // 1. Сначала наполняем (этот цикл НЕ замеряем)
    fill_random(test_data, N);

    // 2. Включаем таймер
    auto start = std::chrono::high_resolution_clock::now();
    
    // 3. ЗАПУСКАЕМ СОРТИРОВКУ (основной цикл алгоритма здесь)
    radix_sort(test_data);
    
    // 4. Выключаем таймер
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> diff = end - start;
    std::cout << "Sorted " << N << " elements in " << diff.count() << " seconds." << std::endl;
}

int main() {
    // Чтобы результаты были разные при каждом запуске (опционально)
    srand(time(NULL)); 

    std::cout << "--- Запуск бенчмарка ---" << std::endl;

    // Теперь мы просто вызываем тесты для разных N
    run_test(100000);   // 100 тысяч
    run_test(500000);   // 500 тысяч
    run_test(1000000);  // 1 миллион (твой основной тест)

    return 0;
}