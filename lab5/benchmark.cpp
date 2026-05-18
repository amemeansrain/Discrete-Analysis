#include <bits/stdc++.h>
using namespace std;

struct Node {
    unordered_map<unsigned char, int> next;

    int l = 0;
    int r = -1;
    int link = -1;
    int parent = -1;

    int suffixIndex = -1;
};

class SuffixTree {
private:
    string s;
    int originalLen;

    vector<Node> tree;

    int root = 0;

    int activeNode = 0;
    int activeEdge = -1;
    int activeLen = 0;

    int remaining = 0;
    int leafEnd = -1;
    int lastNewNode = -1;

    int newNode(int l, int r, int parent) {
        Node v;
        v.l = l;
        v.r = r;
        v.parent = parent;
        v.link = root;

        tree.push_back(std::move(v));
        return (int)tree.size() - 1;
    }

    int edgeLength(int v) const {
        int right = (tree[v].r == -1 ? leafEnd : tree[v].r);
        return right - tree[v].l + 1;
    }

    bool walkDown(int v) {
        int len = edgeLength(v);

        if (activeLen >= len) {
            activeEdge += len;
            activeLen -= len;
            activeNode = v;
            return true;
        }

        return false;
    }

    void extend(int pos) {
        leafEnd = pos;
        ++remaining;
        lastNewNode = -1;

        while (remaining > 0) {
            if (activeLen == 0) {
                activeEdge = pos;
            }

            unsigned char cur = static_cast<unsigned char>(s[activeEdge]);

            auto it = tree[activeNode].next.find(cur);

            if (it == tree[activeNode].next.end()) {
                int leaf = newNode(pos, -1, activeNode);
                tree[activeNode].next[cur] = leaf;

                if (lastNewNode != -1) {
                    tree[lastNewNode].link = activeNode;
                    lastNewNode = -1;
                }
            } else {
                int nxt = it->second;

                if (walkDown(nxt)) {
                    continue;
                }

                if (s[tree[nxt].l + activeLen] == s[pos]) {
                    if (lastNewNode != -1 && activeNode != root) {
                        tree[lastNewNode].link = activeNode;
                        lastNewNode = -1;
                    }

                    ++activeLen;
                    break;
                }

                int splitEnd = tree[nxt].l + activeLen - 1;
                int split = newNode(tree[nxt].l, splitEnd, activeNode);

                tree[activeNode].next[cur] = split;

                tree[nxt].l += activeLen;
                tree[nxt].parent = split;

                tree[split].next[static_cast<unsigned char>(s[tree[nxt].l])] = nxt;

                int leaf = newNode(pos, -1, split);
                tree[split].next[static_cast<unsigned char>(s[pos])] = leaf;

                if (lastNewNode != -1) {
                    tree[lastNewNode].link = split;
                }

                lastNewNode = split;
            }

            --remaining;

            if (activeNode == root && activeLen > 0) {
                --activeLen;
                activeEdge = pos - remaining + 1;
            } else if (activeNode != root) {
                activeNode = tree[activeNode].link;
            }
        }
    }

    void setSuffixIndexByDFS(int v, int depth) {
        if (tree[v].next.empty()) {
            tree[v].suffixIndex = (int)s.size() - depth;
            return;
        }

        for (auto &kv : tree[v].next) {
            int to = kv.second;
            setSuffixIndexByDFS(to, depth + edgeLength(to));
        }
    }

    void collectLeaves(int v, vector<int>& ans) const {
        if (tree[v].next.empty()) {
            int pos = tree[v].suffixIndex;

            if (0 <= pos && pos < originalLen) {
                ans.push_back(pos + 1);
            }

            return;
        }

        for (const auto &kv : tree[v].next) {
            collectLeaves(kv.second, ans);
        }
    }

public:
    explicit SuffixTree(const string& text) {
        originalLen = (int)text.size();

        s = text;
        s.push_back('\0');

        tree.reserve(2 * (int)s.size() + 2);

        tree.push_back(Node());
        tree[root].link = root;

        activeNode = root;

        for (int i = 0; i < (int)s.size(); ++i) {
            extend(i);
        }

        setSuffixIndexByDFS(root, 0);
    }

    vector<int> findOccurrences(const string& pattern) const {
        vector<int> ans;

        if (pattern.empty()) {
            return ans;
        }

        int v = root;
        int i = 0;
        int m = (int)pattern.size();

        while (i < m) {
            unsigned char c = static_cast<unsigned char>(pattern[i]);

            auto it = tree[v].next.find(c);

            if (it == tree[v].next.end()) {
                return ans;
            }

            int to = it->second;
            int len = edgeLength(to);

            for (int j = 0; j < len && i < m; ++j, ++i) {
                if (s[tree[to].l + j] != pattern[i]) {
                    return {};
                }
            }

            if (i == m) {
                collectLeaves(to, ans);
                sort(ans.begin(), ans.end());
                return ans;
            }

            v = to;
        }

        return ans;
    }
};

vector<int> naiveSearch(const string& text, const string& pattern) {
    vector<int> ans;

    if (pattern.empty() || pattern.size() > text.size()) {
        return ans;
    }

    int n = (int)text.size();
    int m = (int)pattern.size();

    for (int i = 0; i + m <= n; ++i) {
        bool ok = true;

        for (int j = 0; j < m; ++j) {
            if (text[i + j] != pattern[j]) {
                ok = false;
                break;
            }
        }

        if (ok) {
            ans.push_back(i + 1);
        }
    }

    return ans;
}

string randomString(int len, int alphabetSize, mt19937& rng) {
    string s;
    s.reserve(len);

    uniform_int_distribution<int> dist(0, alphabetSize - 1);

    for (int i = 0; i < len; ++i) {
        s.push_back(char('a' + dist(rng)));
    }

    return s;
}

int main(int argc, char* argv[]) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int textLen = 100000;
    int queryCount = 1000;
    int minPatternLen = 5;
    int maxPatternLen = 40;
    int alphabetSize = 26;
    int seed = 42;

    if (argc >= 2) textLen = stoi(argv[1]);
    if (argc >= 3) queryCount = stoi(argv[2]);
    if (argc >= 4) minPatternLen = stoi(argv[3]);
    if (argc >= 5) maxPatternLen = stoi(argv[4]);
    if (argc >= 6) alphabetSize = stoi(argv[5]);
    if (argc >= 7) seed = stoi(argv[6]);

    mt19937 rng(seed);

    cout << "Benchmark parameters:\n";
    cout << "text length: " << textLen << '\n';
    cout << "queries: " << queryCount << '\n';
    cout << "pattern length: [" << minPatternLen << ", " << maxPatternLen << "]\n";
    cout << "alphabet size: " << alphabetSize << '\n';
    cout << "seed: " << seed << "\n\n";

    string text = randomString(textLen, alphabetSize, rng);

    vector<string> patterns;
    patterns.reserve(queryCount);

    uniform_int_distribution<int> lenDist(minPatternLen, maxPatternLen);
    uniform_int_distribution<int> typeDist(0, 99);

    for (int i = 0; i < queryCount; ++i) {
        int len = lenDist(rng);

        if (len > textLen) {
            len = textLen;
        }

        int type = typeDist(rng);

        if (type < 70) {
            // 70% образцов точно являются подстроками текста
            uniform_int_distribution<int> startDist(0, textLen - len);
            int start = startDist(rng);
            patterns.push_back(text.substr(start, len));
        } else {
            // 30% полностью случайные образцы
            patterns.push_back(randomString(len, alphabetSize, rng));
        }
    }

    auto buildStart = chrono::high_resolution_clock::now();

    SuffixTree tree(text);

    auto buildEnd = chrono::high_resolution_clock::now();

    double buildMs = chrono::duration<double, milli>(buildEnd - buildStart).count();

    vector<vector<int>> suffixTreeResults;
    suffixTreeResults.reserve(queryCount);

    size_t suffixChecksum = 0;

    auto suffixStart = chrono::high_resolution_clock::now();

    for (const string& pattern : patterns) {
        vector<int> result = tree.findOccurrences(pattern);
        suffixChecksum += result.size();
        suffixTreeResults.push_back(std::move(result));
    }

    auto suffixEnd = chrono::high_resolution_clock::now();

    double suffixSearchMs = chrono::duration<double, milli>(suffixEnd - suffixStart).count();

    size_t naiveChecksum = 0;
    bool correct = true;

    auto naiveStart = chrono::high_resolution_clock::now();

    for (int i = 0; i < queryCount; ++i) {
        vector<int> result = naiveSearch(text, patterns[i]);
        naiveChecksum += result.size();

        if (result != suffixTreeResults[i]) {
            correct = false;

            cerr << "Mismatch on query #" << i + 1 << '\n';
            cerr << "pattern = " << patterns[i] << '\n';

            cerr << "suffix tree answer size = " << suffixTreeResults[i].size() << '\n';
            cerr << "naive answer size = " << result.size() << '\n';

            break;
        }
    }

    auto naiveEnd = chrono::high_resolution_clock::now();

    double naiveSearchMs = chrono::duration<double, milli>(naiveEnd - naiveStart).count();

    cout << fixed << setprecision(3);

    cout << "Results:\n";
    cout << "suffix tree build time:  " << buildMs << " ms\n";
    cout << "suffix tree search time: " << suffixSearchMs << " ms\n";
    cout << "naive search time:       " << naiveSearchMs << " ms\n\n";

    cout << "total suffix tree time:  " << buildMs + suffixSearchMs << " ms\n";

    if (suffixSearchMs > 0) {
        cout << "search speedup only:     " << naiveSearchMs / suffixSearchMs << "x\n";
    }

    if (buildMs + suffixSearchMs > 0) {
        cout << "total speedup:           " << naiveSearchMs / (buildMs + suffixSearchMs) << "x\n";
    }

    cout << '\n';

    cout << "suffix checksum: " << suffixChecksum << '\n';
    cout << "naive checksum:  " << naiveChecksum << '\n';
    cout << "answers correct: " << (correct ? "YES" : "NO") << '\n';

    return 0;
}