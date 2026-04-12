# Компилятор
CXX = g++

# Флаги (C++17, оптимизация O2, строгие проверки)
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -Wshadow -Wconversion

# Имена исполняемых файлов
TARGET = patricia
BENCH_TARGET = bench_patricia

# Исходные файлы
SRC = main.cpp
BENCH_SRC = benchmark.cpp

# Количество тестов по умолчанию для питон-скрипта
COUNT ?= 1000

# ----------- Команды -----------

.PHONY: all run run_file run_io test bench clean rebuild

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

# Сборка и запуск БЕНЧМАРКА
bench: $(BENCH_SRC)
	$(CXX) $(CXXFLAGS) -o $(BENCH_TARGET) $(BENCH_SRC)
	./$(BENCH_TARGET)

run: $(TARGET)
	./$(TARGET)

run_file: $(TARGET)
	./$(TARGET) < input.txt

run_io: $(TARGET)
	./$(TARGET) < input.txt > output.txt

test: $(TARGET)
	@echo "--- Сборка тестов: $(COUNT) операций ---"
	@python3 gen_test.py $(COUNT)
	@echo "--- Запуск программы ---"
	@time ./$(TARGET) < input.txt > output.txt
	@echo "--- Проверка результатов ---"
	@diff output.txt expected.txt && echo "РЕЗУЛЬТАТ: OK (Тесты пройдены) 🎉" || echo "РЕЗУЛЬТАТ: ОШИБКА (Есть расхождения) ❌"

clean:
	rm -f $(TARGET) $(BENCH_TARGET) input.txt output.txt expected.txt

rebuild: clean all