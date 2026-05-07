CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -Wpedantic

TARGET = solution
BENCH = benchmark

SRC = main.cpp
BENCH_SRC = benchmark.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

$(BENCH): $(BENCH_SRC)
	$(CXX) $(CXXFLAGS) $(BENCH_SRC) -o $(BENCH)

run: $(TARGET)
	./$(TARGET)

bench: $(TARGET) $(BENCH)
	./$(BENCH)

clean:
	rm -f $(TARGET) $(BENCH) benchmark_input.txt benchmark_output.txt benchmark_results.csv