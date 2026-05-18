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

static void removeTrailingCR(string& x) {
    if (!x.empty() && x.back() == '\r') {
        x.pop_back();
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string text;

    if (!getline(cin, text)) {
        return 0;
    }

    removeTrailingCR(text);

    SuffixTree st(text);

    string pattern;
    int number = 1;

    while (getline(cin, pattern)) {
        removeTrailingCR(pattern);

        vector<int> occurrences = st.findOccurrences(pattern);

        if (!occurrences.empty()) {
            cout << number << ": ";

            for (size_t i = 0; i < occurrences.size(); ++i) {
                if (i) {
                    cout << ", ";
                }

                cout << occurrences[i];
            }

            cout << '\n';
        }

        ++number;
    }

    return 0;
}