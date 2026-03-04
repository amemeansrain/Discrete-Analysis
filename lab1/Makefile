# Компилятор и флаги
CXX = g++
CXXFLAGS = -O3 -Wall -Wextra -std=c++17

# Объектные файлы для логики (используются в обоих таргетах)
LOGIC_OBJ = sort_logic.o

# Цель по умолчанию: собрать solution и benchmark
all: solution benchmark

# Сборка основной программы
solution: main.o $(LOGIC_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Сборка бенчмарка
benchmark: benchmark.o $(LOGIC_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Компиляция main.cpp
main.o: main.cpp include/sort_logic.h include/my_vector.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Компиляция benchmark.cpp
benchmark.o: src/benchmark.cpp include/sort_logic.h include/my_vector.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Компиляция sort_logic.cpp
sort_logic.o: src/sort_logic.cpp include/sort_logic.h include/my_vector.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Цель для тестирования (требует Python 3)
test: solution
	@echo "Генерация данных..."
	@python3 py/gen.py 100000 > test_input.txt
	@echo "Запуск сортировки..."
	@./solution < test_input.txt > test_output.txt
	@echo "Проверка результата..."
	@python3 py/check.py < test_output.txt
	# @rm -f test_input.txt test_output.txt

# Очистка
clean:
	rm -f *.o solution benchmark

# Объявляем, что эти цели не являются файлами
.PHONY: all test clean