CXX = g++
CXXFLAGS = -std=c++17 -O2 -pipe -march=native -Wall -Wextra

TARGET = benchmark
SRC = benchmark.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET) 0

by-n: $(TARGET)
	./$(TARGET) 1

by-m: $(TARGET)
	./$(TARGET) 2

clean:
	rm -f $(TARGET)

.PHONY: all run by-n by-m clean