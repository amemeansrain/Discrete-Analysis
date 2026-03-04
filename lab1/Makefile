CXX = g++
CXXFLAGS = -O3 -Wall -Wextra -std=c++17

LOGIC_OBJ = sort_logic.o

all: solution benchmark

solution: main.o $(LOGIC_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

benchmark: benchmark.o $(LOGIC_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

main.o: main.cpp include/sort_logic.h include/my_vector.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

benchmark.o: src/benchmark.cpp include/sort_logic.h include/my_vector.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

sort_logic.o: src/sort_logic.cpp include/sort_logic.h include/my_vector.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: solution
	@echo "Генерация данных..."
	@python3 py/gen.py 100000 > test_input.txt
	@echo "Запуск сортировки..."
	@./solution < test_input.txt > test_output.txt
	@echo "Проверка результата..."
	@python3 py/check.py < test_output.txt

clean:
	rm -f *.o solution benchmark

.PHONY: all test clean