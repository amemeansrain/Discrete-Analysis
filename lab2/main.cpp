#include <bits/stdc++.h>
#include <cerrno>
#include <cstring>
using namespace std;

struct Node {
    string key;
    uint64_t value;
    int bitIndex;
    Node* left;
    Node* right;

    Node(string k, uint64_t v, int b)
        : key(k), value(v), bitIndex(b), left(nullptr), right(nullptr) {}
};

struct Patricia {
    Node* header;

    Patricia() {
        header = new Node("", 0, -1);
        header->left = header;
        header->right = header;
    }

    ~Patricia() {
        // Итеративная очистка вместо рекурсии
        clearIterative();
        delete header;
    }

    // Итеративное удаление всех узлов, кроме header
    void clearIterative() {
        if (header->left == header) return;

        stack<Node*> stk;
        unordered_set<Node*> visited;
        stk.push(header->left);

        while (!stk.empty()) {
            Node* node = stk.top();
            if (!node || visited.count(node)) {
                stk.pop();
                continue;
            }

            bool pushed = false;
            if (node->left && node->left->bitIndex > node->bitIndex && !visited.count(node->left)) {
                stk.push(node->left);
                pushed = true;
            }
            if (node->right && node->right->bitIndex > node->bitIndex && !visited.count(node->right)) {
                stk.push(node->right);
                pushed = true;
            }

            if (!pushed) {
                visited.insert(node);
                stk.pop();
                delete node;
            }
        }
    }

    static string toLower(string s) {
        for (char& c : s) c = tolower(c);
        return s;
    }

    static int getBit(const string& s, int bit) {
        // bit всегда >= 0 (кроме случаев ошибок, которые мы исправили)
        int byte = bit / 8;
        int offset = 7 - (bit % 8);
        if (byte >= (int)s.size()) return 0;
        return ((unsigned char)s[byte] >> offset) & 1;
    }

    static int firstDiff(const string& a, const string& b) {
        int maxLen = max(a.size(), b.size()) * 8;
        for (int i = 0; i < maxLen; i++) {
            if (getBit(a, i) != getBit(b, i))
                return i;
        }
        return -1;
    }

    Node* search(const string& key) {
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

    bool find(const string& k, uint64_t& val) {
        string key = toLower(k);
        Node* res = search(key);
        if (res != header && res->key == key) {
            val = res->value;
            return true;
        }
        return false;
    }

    bool insert(const string& k, uint64_t val) {
        string key = toLower(k);

        Node* found = search(key);
        if (found != header && found->key == key)
            return false;

        int diffBit;
        if (found == header)
            diffBit = 0;
        else
            diffBit = firstDiff(key, found->key);

        Node* parent = header;
        Node* curr = header->left;

        while (parent->bitIndex < curr->bitIndex &&
               curr->bitIndex < diffBit) {
            parent = curr;
            if (getBit(key, curr->bitIndex))
                curr = curr->right;
            else
                curr = curr->left;
        }

        Node* node = new Node(key, val, diffBit);

        if (getBit(key, diffBit)) {
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

        Node* grand = header;
        Node* parent = header;
        Node* curr = header->left;

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

        // Случай: удаляемый узел — единственный (нет потомков с бо́льшим bitIndex)
        if (curr == parent) {
            if (curr == header->left) {
                header->left = header;
            } else {
                if (getBit(key, parent->bitIndex))
                    grand->right = header;
                else
                    grand->left = header;
            }
            delete curr;
            return true;
        }

        // Копируем данные из parent в curr
        curr->key = parent->key;
        curr->value = parent->value;

        // Определяем потомка parent, который станет на место parent
        Node* child;
        if (getBit(parent->key, parent->bitIndex))
            child = parent->left;   // правый указатель parent указывал на parent
        else
            child = parent->right;  // левый указатель parent указывал на parent

        // Подвешиваем child к grand
        // ИСПРАВЛЕНО: если grand == header, то модифицируем header->left (корень всегда там)
        if (grand == header) {
            grand->left = child;
        } else {
            if (getBit(parent->key, grand->bitIndex))
                grand->right = child;
            else
                grand->left = child;
        }

        delete parent;
        return true;
    }

    // Итеративный сбор всех пар (key, value)
    void collectIterative(vector<pair<string,uint64_t>>& res) {
        if (header->left == header) return;

        stack<Node*> stk;
        unordered_set<Node*> visited;
        stk.push(header->left);

        while (!stk.empty()) {
            Node* node = stk.top();
            if (!node || visited.count(node)) {
                stk.pop();
                continue;
            }

            bool pushed = false;
            if (node->left && node->left->bitIndex > node->bitIndex && !visited.count(node->left)) {
                stk.push(node->left);
                pushed = true;
            }
            if (node->right && node->right->bitIndex > node->bitIndex && !visited.count(node->right)) {
                stk.push(node->right);
                pushed = true;
            }

            if (!pushed) {
                visited.insert(node);
                stk.pop();
                if (node != header)
                    res.emplace_back(node->key, node->value);
            }
        }
    }

    bool save(const string& path, string& err) {
        ofstream out(path, ios::binary);
        if (!out) {
            err = strerror(errno);
            return false;
        }

        vector<pair<string,uint64_t>> data;
        collectIterative(data);   // используем итеративную версию

        uint64_t sz = data.size();
        if (!out.write((char*)&sz, sizeof(sz))) {
            err = strerror(errno);
            return false;
        }

        for (auto& [k, v] : data) {
            uint64_t len = k.size();
            if (!out.write((char*)&len, sizeof(len)) ||
                !out.write(k.data(), len) ||
                !out.write((char*)&v, sizeof(v))) {
                err = strerror(errno);
                return false;
            }
        }
        return true;
    }

    bool load(const string& path, string& err) {
        ifstream in(path, ios::binary);
        if (!in) {
            err = strerror(errno);
            return false;
        }

        Patricia temp;

        uint64_t sz;
        if (!in.read((char*)&sz, sizeof(sz))) {
            err = "bad file format";
            return false;
        }

        for (uint64_t i = 0; i < sz; i++) {
            uint64_t len;
            if (!in.read((char*)&len, sizeof(len))) {
                err = "bad file format";
                return false;
            }
            if (len > 256) {
                err = "bad file format";
                return false;
            }
            string key(len, '\0');
            if (!in.read(&key[0], len)) {
                err = "bad file format";
                return false;
            }
            uint64_t val;
            if (!in.read((char*)&val, sizeof(val))) {
                err = "bad file format";
                return false;
            }
            if (!temp.insert(key, val)) {
                err = "bad file format";
                return false;
            }
        }

        if (in.peek() != EOF) {
            err = "bad file format";
            return false;
        }

        swap(header, temp.header);
        return true;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    Patricia dict;
    string line;

    while (getline(cin, line)) {
        try {
            if (line.empty()) continue;

            if (line[0] == '+') {
                string word;
                uint64_t val;
                istringstream ss(line.substr(1));
                ss >> word >> val;

                if (dict.insert(word, val))
                    cout << "OK\n";
                else
                    cout << "Exist\n";
            }
            else if (line[0] == '-') {
                string word = line.substr(2);

                if (dict.erase(word))
                    cout << "OK\n";
                else
                    cout << "NoSuchWord\n";
            }
            else if (line[0] == '!') {
                string bang, cmd, path;
                istringstream ss(line);
                ss >> bang >> cmd >> path;

                string err;

                if (cmd == "Save") {
                    if (dict.save(path, err))
                        cout << "OK\n";
                    else
                        cout << "ERROR: " << err << "\n";
                } else if (cmd == "Load") {
                    if (dict.load(path, err))
                        cout << "OK\n";
                    else
                        cout << "ERROR: " << err << "\n";
                }
            }
            else {
                string word = line;
                uint64_t val;

                if (dict.find(word, val))
                    cout << "OK: " << val << "\n";
                else
                    cout << "NoSuchWord\n";
            }
        }
        catch (...) {
            cout << "ERROR: exception\n";
        }
    }
    return 0;
}