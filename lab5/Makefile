CXX = g++
CXXFLAGS = -std=c++17 -O2 -pipe -march=native -Wall -Wextra

TARGET = benchmark
SRC = benchmark.cpp

TEXT_LEN ?= 100000
QUERY_COUNT ?= 1000
MIN_PATTERN_LEN ?= 5
MAX_PATTERN_LEN ?= 40
ALPHABET_SIZE ?= 26
SEED ?= 42

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

bench: $(TARGET)
	./$(TARGET) $(TEXT_LEN) $(QUERY_COUNT) $(MIN_PATTERN_LEN) $(MAX_PATTERN_LEN) $(ALPHABET_SIZE) $(SEED)

small: $(TARGET)
	./$(TARGET) 10000 100 3 20 26 42

medium: $(TARGET)
	./$(TARGET) 100000 1000 5 40 26 42

large: $(TARGET)
	./$(TARGET) 300000 5000 5 60 26 42

clean:
	rm -f $(TARGET)

.PHONY: all run bench small medium large clean