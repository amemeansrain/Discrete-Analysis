# компилятор
CXX = g++

# флаги (быстро + строго)
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -Wshadow -Wconversion

# имя программы
TARGET = patricia

# исходники
SRC = main.cpp

# ----------- команды -----------

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

# запуск с вводом
run: $(TARGET)
	./$(TARGET)

# запуск с файлом
run_file: $(TARGET)
	./$(TARGET) < input.txt

# запуск с выводом в файл
run_io: $(TARGET)
	./$(TARGET) < input.txt > output.txt

# очистка
clean:
	rm -f $(TARGET)

# пересборка
rebuild: clean all