#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <map>
#include <algorithm>
#include <iomanip>

using namespace std;
using namespace std::chrono;

// ================= PATRICIA =================

struct Node {
    string key;
    uint64_t value;
    int bitIndex;
    Node* left;
    Node* right;

    Node(string k, uint64_t v, int b)
        : key(k), value(v), bitIndex(b), left(this), right(this) {}
};

class Patricia {
private:
    Node* header;

    static string toLower(string s) {
        for (char& c : s)
            c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
        return s;
    }

    static int getBit(const string& s, int i) {
        int byte = i / 8;
        int bit = 7 - (i % 8);
        if (byte >= (int)s.size()) return 0;
        return (s[byte] >> bit) & 1;
    }

    static int firstDiff(const string& a, const string& b) {
        int maxLen = static_cast<int>(max(a.size(), b.size()) * 8);
        for (int i = 0; i < maxLen; i++) {
            if (getBit(a, i) != getBit(b, i))
                return i;
        }
        return 0;
    }

    Node* searchNode(const string& key) const {
        Node* parent = header;
        Node* curr = header->left;

        while (parent->bitIndex < curr->bitIndex) {
            parent = curr;
            if (getBit(key, curr->bitIndex))
                curr = curr->right;
            else
                curr = curr->left;
        }
        return curr;
    }

public:
    Patricia() {
        header = new Node("", 0, -1);
        header->left = header;
        header->right = header;
    }

    bool find(const string& k, uint64_t& val) const {
        string key = toLower(k);
        Node* res = searchNode(key);
        if (res != header && res->key == key) {
            val = res->value;
            return true;
        }
        return false;
    }

    bool insert(const string& k, uint64_t val) {
        string key = toLower(k);

        Node* found = searchNode(key);
        if (found != header && found->key == key)
            return false;

        int diff = firstDiff(key, found->key);

        Node* parent = header;
        Node* curr = header->left;

        while (parent->bitIndex < curr->bitIndex &&
               curr->bitIndex < diff) {
            parent = curr;
            if (getBit(key, curr->bitIndex))
                curr = curr->right;
            else
                curr = curr->left;
        }

        Node* node = new Node(key, val, diff);

        if (getBit(key, diff)) {
            node->left = curr;
            node->right = node;
        } else {
            node->left = node;
            node->right = curr;
        }

        if (parent == header || getBit(key, parent->bitIndex) == 0)
            parent->left = node;
        else
            parent->right = node;

        return true;
    }

    bool erase(const string& k) {
        string key = toLower(k);

        Node *grand = header, *parent = header, *curr = header->left;

        while (parent->bitIndex < curr->bitIndex) {
            grand = parent;
            parent = curr;
            if (getBit(key, curr->bitIndex))
                curr = curr->right;
            else
                curr = curr->left;
        }

        if (curr == header || curr->key != key)
            return false;

        if (curr == parent) {
            if (curr == header->left)
                header->left = header;
            else {
                if (getBit(key, grand->bitIndex))
                    grand->right = header;
                else
                    grand->left = header;
            }
            delete curr;
            return true;
        }

        curr->key = parent->key;
        curr->value = parent->value;

        Node* child;
        if (getBit(parent->key, parent->bitIndex))
            child = parent->left;
        else
            child = parent->right;

        if (getBit(parent->key, grand->bitIndex))
            grand->right = child;
        else
            grand->left = child;

        delete parent;
        return true;
    }
};

// ================= BENCH =================

string gen_random_string(int len) {
    static const char alphanum[] = "abcdefghijklmnopqrstuvwxyz";
    string s;
    s.reserve(len);
    for (int i = 0; i < len; ++i)
        s += alphanum[rand() % 26];
    return s;
}

void run_total_bench(int N) {
    vector<string> words;
    words.reserve(N);

    for (int i = 0; i < N; ++i)
        words.push_back(gen_random_string(rand() % 33)); // короче строки → честнее

    // --- PATRICIA ---
    auto p_start = high_resolution_clock::now();
    {
        Patricia pat;
        uint64_t val;

        for (int i = 0; i < N; ++i)
            pat.insert(words[i], i);

        for (int i = 0; i < N; ++i)
            pat.find(words[i], val);

        for (int i = 0; i < N; ++i)
            pat.erase(words[i]);
    }
    auto p_end = high_resolution_clock::now();

    // --- STD::MAP ---
    auto m_start = high_resolution_clock::now();
    {
        map<string, uint64_t> mp;

        for (int i = 0; i < N; ++i)
            mp[words[i]] = i;

        for (int i = 0; i < N; ++i)
            mp.find(words[i]);

        for (int i = 0; i < N; ++i)
            mp.erase(words[i]);
    }
    auto m_end = high_resolution_clock::now();

    auto p_total = duration_cast<milliseconds>(p_end - p_start).count();
    auto m_total = duration_cast<milliseconds>(m_end - m_start).count();

    cout << "| " << setw(10) << N
         << " | " << setw(15) << p_total << " ms | "
         << setw(15) << m_total << " ms |" << endl;
}

int main() {
    srand(42);

    cout << "--------------------------------------------------------\n";
    cout << "| Elements   | Patricia           | std::map           |\n";
    cout << "--------------------------------------------------------\n";

    vector<int> sizes = {100000, 500000, 1000000};

    for (int n : sizes)
        run_total_bench(n);

    cout << "--------------------------------------------------------\n";
}