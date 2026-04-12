# Компилятор
CXX = g++

# Флаги (C++17, оптимизация O2, строгие проверки)
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -Wshadow -Wconversion

# Имя исполняемого файла
TARGET = patricia

# Исходный файл
SRC = main.cpp

# Количество тестов по умолчанию (можно менять при запуске: make test COUNT=5000)
COUNT ?= 1000

# ----------- Команды -----------

.PHONY: all run run_file run_io test clean rebuild

# Сборка программы
all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

# Обычный запуск с ручным вводом в консоль
run: $(TARGET)
	./$(TARGET)

# Запуск с чтением из input.txt
run_file: $(TARGET)
	./$(TARGET) < input.txt

# Запуск с чтением из input.txt и записью в output.txt
run_io: $(TARGET)
	./$(TARGET) < input.txt > output.txt

# АВТОМАТИЧЕСКОЕ ТЕСТИРОВАНИЕ
# 1. Генерирует COUNT тестов через Python
# 2. Прогоняет программу (замеряя время)
# 3. Сравнивает вывод с эталоном через diff
test: $(TARGET)
	@echo "--- Сборка тестов: $(COUNT) операций ---"
	@python3 gen_test.py $(COUNT)
	@echo "--- Запуск программы ---"
	@time ./$(TARGET) < input.txt > output.txt
	@echo "--- Проверка результатов ---"
	@diff output.txt expected.txt && echo "РЕЗУЛЬТАТ: OK (Тесты пройдены) 🎉" || echo "РЕЗУЛЬТАТ: ОШИБКА (Есть расхождения) ❌"

# Очистка всех бинарников и временных файлов тестов
clean:
	rm -f $(TARGET) input.txt output.txt expected.txt

# Полная пересборка
rebuild: clean all