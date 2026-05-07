#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;

const string EXECUTABLE = "./solution";
const string INPUT_FILE = "benchmark_input.txt";
const string OUTPUT_FILE = "benchmark_output.txt";

struct Position {
    int line;
    int word;

    bool operator<(const Position& other) const {
        if (line != other.line)
            return line < other.line;
        return word < other.word;
    }

    bool operator==(const Position& other) const {
        return line == other.line && word == other.word;
    }
};

struct TestCase {
    string name;
    vector<string> pattern;
    vector<string> text;
    int lineWidth;
};

TestCase generateWorstCaseNaive() {
    TestCase t;
    t.name = "worst_case_naive";
    t.lineWidth = 100;

    t.pattern = {
        "a","a","a","a","a",
        "a","a","a","a","b"
    };

    t.text = vector<string>(500000, "a");

    return t;
}

TestCase generateLongPatternCase() {
    TestCase t;
    t.name = "long_pattern";
    t.lineWidth = 100;

    t.pattern = vector<string>(200, "x");
    t.pattern.back() = "y";

    t.text = vector<string>(1000000, "x");

    return t;
}

void writeInputFile(const TestCase& test) {
    ofstream out(INPUT_FILE);

    for (int i = 0; i < (int)test.pattern.size(); i++) {
        if (i) out << " ";
        out << test.pattern[i];
    }

    out << '\n';

    for (int i = 0; i < (int)test.text.size(); i++) {
        if (i && i % test.lineWidth == 0) {
            out << '\n';
        } else if (i) {
            out << " ";
        }

        out << test.text[i];
    }

    out << '\n';
}

vector<Position> naiveSearch(const TestCase& test) {
    vector<Position> result;

    int n = test.text.size();
    int m = test.pattern.size();

    for (int i = 0; i + m <= n; i++) {
        bool match = true;

        for (int j = 0; j < m; j++) {
            if (test.text[i + j] != test.pattern[j]) {
                match = false;
                break;
            }
        }

        if (match) {
            result.push_back({
                i / test.lineWidth + 1,
                i % test.lineWidth + 1
            });
        }
    }

    return result;
}

vector<Position> runYourAlgorithm() {
    vector<Position> result;

    string command = EXECUTABLE + " < " + INPUT_FILE + " > " + OUTPUT_FILE;
    system(command.c_str());

    ifstream in(OUTPUT_FILE);

    string line;

    while (getline(in, line)) {
        if (line.empty())
            continue;

        for (char& c : line) {
            if (c == ',')
                c = ' ';
        }

        stringstream ss(line);

        Position p;
        ss >> p.line >> p.word;

        result.push_back(p);
    }

    return result;
}

double benchmarkAG(const TestCase& test, int runs) {
    writeInputFile(test);

    double total = 0;

    for (int i = 0; i < runs; i++) {
        auto start = high_resolution_clock::now();

        runYourAlgorithm();

        auto finish = high_resolution_clock::now();

        duration<double> elapsed = finish - start;
        total += elapsed.count();
    }

    return total / runs;
}

double benchmarkNaive(const TestCase& test, int runs) {
    double total = 0;

    for (int i = 0; i < runs; i++) {
        auto start = high_resolution_clock::now();

        naiveSearch(test);

        auto finish = high_resolution_clock::now();

        duration<double> elapsed = finish - start;
        total += elapsed.count();
    }

    return total / runs;
}

void verifyCorrectness(const TestCase& test) {
    writeInputFile(test);

    auto ag = runYourAlgorithm();
    auto naive = naiveSearch(test);

    sort(ag.begin(), ag.end());
    sort(naive.begin(), naive.end());

    if (ag != naive) {
        cout << "WRONG ANSWER on test: " << test.name << "\n";
        exit(1);
    }
}

int main() {
    vector<TestCase> tests;

    tests.push_back(generateWorstCaseNaive());
    tests.push_back(generateLongPatternCase());

    cout << "=====================================================\n";
    cout << "Benchmark Apostolico-Giancarlo vs Naive Search\n";
    cout << "=====================================================\n\n";

    for (auto& test : tests) {
        cout << "Running test: " << test.name << "\n";

        verifyCorrectness(test);

        double agTime = benchmarkAG(test, 3);
        double naiveTime = benchmarkNaive(test, 3);

        cout << "Text size:    " << test.text.size() << "\n";
        cout << "Pattern size: " << test.pattern.size() << "\n";

        cout << "AG:           " << agTime << " sec\n";
        cout << "Naive:        " << naiveTime << " sec\n";

        cout << "---------------------------------------------\n";
    }

    return 0;
}