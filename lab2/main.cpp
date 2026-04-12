#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstdint>
#include <cerrno>
#include <cstring>

using namespace std;

struct Node {
    string text;
    uint64_t val;
    bool is_word;
    vector<pair<char, Node*>> children;

    Node(string t = "", uint64_t v = 0, bool w = false)
        : text(t), val(v), is_word(w) {}
};

class Patricia {
private:
    Node* root;

    void destroy(Node* curr) {
        if (!curr) return;
        for (auto& edge : curr->children) destroy(edge.second);
        delete curr;
    }

    bool erase_rec(Node* curr, const string& key, size_t i) {
        if (i == key.length()) {
            if (!curr->is_word) return false;
            curr->is_word = false;
            return true;
        }
        char c = key[i];
        for (size_t k = 0; k < curr->children.size(); ++k) {
            if (curr->children[k].first == c) {
                Node* next_node = curr->children[k].second;
                size_t j = 0;
                while (j < next_node->text.length() && i + j < key.length() && next_node->text[j] == key[i + j]) j++;
                if (j != next_node->text.length()) return false;

                if (erase_rec(next_node, key, i + j)) {
                    if (!next_node->is_word && next_node->children.empty()) {
                        delete next_node;
                        curr->children.erase(curr->children.begin() + static_cast<long>(k));
                    } else if (!next_node->is_word && next_node->children.size() == 1) {
                        Node* only_child = next_node->children[0].second;
                        next_node->text += only_child->text;
                        next_node->is_word = only_child->is_word;
                        next_node->val = only_child->val;
                        next_node->children = move(only_child->children);
                        delete only_child;
                    }
                    return true;
                }
                return false;
            }
        }
        return false;
    }

    void collect_all(Node* curr, string prefix, vector<pair<string, uint64_t>>& res) const {
        if (curr->is_word) res.push_back({prefix, curr->val});
        for (const auto& edge : curr->children) collect_all(edge.second, prefix + edge.second->text, res);
    }

public:
    Patricia() { root = new Node(); }
    ~Patricia() { destroy(root); }

    void clear() {
        destroy(root);
        root = new Node();
    }

    bool insert(const string& key, uint64_t val) {
        Node* curr = root;
        size_t i = 0;
        while (i < key.length()) {
            char c = key[i];
            Node* next_node = nullptr;
            for (auto& edge : curr->children) { if (edge.first == c) { next_node = edge.second; break; } }

            if (!next_node) {
                curr->children.push_back({c, new Node(key.substr(i), val, true)});
                return true;
            }

            size_t j = 0;
            while (j < next_node->text.length() && i + j < key.length() && next_node->text[j] == key[i + j]) j++;

            if (j == next_node->text.length()) {
                i += j;
                if (i == key.length()) {
                    if (next_node->is_word) return false;
                    next_node->is_word = true; next_node->val = val; return true;
                }
                curr = next_node;
            } else {
                Node* split = new Node(next_node->text.substr(j), next_node->val, next_node->is_word);
                split->children = move(next_node->children);
                next_node->text = next_node->text.substr(0, j);
                next_node->is_word = false;
                next_node->children.clear();
                next_node->children.push_back({split->text[0], split});
                if (i + j == key.length()) { next_node->is_word = true; next_node->val = val; }
                else next_node->children.push_back({key[i + j], new Node(key.substr(i + j), val, true)});
                return true;
            }
        }
        return false;
    }

    bool find(const string& key, uint64_t& out_val) const {
        Node* curr = root; size_t i = 0;
        while (i < key.length()) {
            char c = key[i]; Node* next = nullptr;
            for (auto& edge : curr->children) if (edge.first == c) next = edge.second;
            if (!next) return false;
            size_t j = 0;
            while (j < next->text.length() && i + j < key.length() && next->text[j] == key[i + j]) j++;
            if (j != next->text.length()) return false;
            i += j; curr = next;
        }
        if (curr->is_word) { out_val = curr->val; return true; }
        return false;
    }

    bool erase(const string& key) { return erase_rec(root, key, 0); }

    bool save(const string& path, string& err) const {
        if (path.empty()) { err = "No such file or directory"; return false; }
        ofstream out(path, ios::binary);
        if (!out) { err = strerror(errno); return false; }
        vector<pair<string, uint64_t>> data;
        collect_all(root, "", data);
        uint64_t sz = data.size();
        out.write((char*)&sz, sizeof(sz));
        for (auto& p : data) {
            uint32_t len = (uint32_t)p.first.length();
            out.write((char*)&len, sizeof(len));
            out.write(p.first.data(), len);
            out.write((char*)&p.second, sizeof(p.second));
        }
        return true;
    }

    bool load(const string& path, string& err) {
        if (path.empty()) { err = "No such file or directory"; return false; }
        ifstream in(path, ios::binary);
        if (!in) { clear(); return true; } // Важное условие: нет файла -> пустое дерево и OK

        Patricia temp; uint64_t sz;
        if (!in.read((char*)&sz, sizeof(sz))) { clear(); return true; }
        for (uint64_t i = 0; i < sz; i++) {
            uint32_t len; if (!in.read((char*)&len, sizeof(len))) return false;
            string k(len, '\0'); if (!in.read(&k[0], len)) return false;
            uint64_t v; if (!in.read((char*)&v, sizeof(v))) return false;
            temp.insert(k, v);
        }
        swap(root, temp.root);
        return true;
    }
};

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    Patricia dict; string line;
    while (getline(cin, line)) {
        if (line.empty()) continue;
        istringstream iss(line); string cmd; iss >> cmd;
        if (cmd == "+") {
            string w; uint64_t v;
            if (iss >> w >> v) {
                for (auto& c : w) c = (char)tolower(c);
                if (dict.insert(w, v)) cout << "OK\n"; else cout << "Exist\n";
            }
        } else if (cmd == "-") {
            string w; if (iss >> w) {
                for (auto& c : w) c = (char)tolower(c);
                if (dict.erase(w)) cout << "OK\n"; else cout << "NoSuchWord\n";
            }
        } else if (cmd == "!") {
            string act, path; iss >> act;
            if (!(iss >> path)) path = ""; // Обработка случая без пути
            string err;
            if (act == "Save") {
                if (dict.save(path, err)) cout << "OK\n"; else cout << "ERROR: " << err << "\n";
            } else if (act == "Load") {
                if (dict.load(path, err)) cout << "OK\n"; else cout << "ERROR: " << err << "\n";
            }
        } else {
            string w = cmd; for (auto& c : w) c = (char)tolower(c);
            uint64_t v; if (dict.find(w, v)) cout << "OK: " << v << "\n"; else cout << "NoSuchWord\n";
        }
    }
    return 0;
}