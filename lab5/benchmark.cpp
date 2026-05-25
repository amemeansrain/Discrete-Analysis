#include <bits/stdc++.h>
using namespace std;

string s;
vector<int> sa, rnk, lcp, lg;
vector<vector<int>> st;

volatile size_t global_checksum = 0;

vector<int> build_sa(string t) {
    t += '\0';

    int n = t.size();
    vector<int> p(n), c(n), pn(n), cn(n), cnt(max(256, n), 0);

    for (unsigned char ch : t) cnt[ch]++;
    for (int i = 1; i < 256; ++i) cnt[i] += cnt[i - 1];
    for (int i = 0; i < n; ++i) p[--cnt[(unsigned char)t[i]]] = i;

    int cls = 1;
    c[p[0]] = 0;

    for (int i = 1; i < n; ++i) {
        if (t[p[i]] != t[p[i - 1]]) ++cls;
        c[p[i]] = cls - 1;
    }

    for (int len = 1; len < n && cls < n; len <<= 1) {
        for (int i = 0; i < n; ++i) {
            pn[i] = p[i] - len;
            if (pn[i] < 0) pn[i] += n;
        }

        fill(cnt.begin(), cnt.begin() + cls, 0);

        for (int x : pn) cnt[c[x]]++;
        for (int i = 1; i < cls; ++i) cnt[i] += cnt[i - 1];

        for (int i = n - 1; i >= 0; --i) {
            int x = pn[i];
            p[--cnt[c[x]]] = x;
        }

        cn[p[0]] = 0;
        int ncls = 1;

        for (int i = 1; i < n; ++i) {
            int a1 = c[p[i]];
            int a2 = c[(p[i] + len) % n];
            int b1 = c[p[i - 1]];
            int b2 = c[(p[i - 1] + len) % n];

            if (a1 != b1 || a2 != b2) ++ncls;
            cn[p[i]] = ncls - 1;
        }

        c.swap(cn);
        cls = ncls;
    }

    p.erase(p.begin());
    return p;
}

void build_lcp() {
    int n = s.size();

    rnk.assign(n, 0);
    lcp.assign(n, 0);

    for (int i = 0; i < n; ++i) {
        rnk[sa[i]] = i;
    }

    int k = 0;

    for (int i = 0; i < n; ++i) {
        int pos = rnk[i];

        if (pos == 0) continue;

        int j = sa[pos - 1];

        while (i + k < n && j + k < n && s[i + k] == s[j + k]) {
            ++k;
        }

        lcp[pos] = k;

        if (k) --k;
    }
}

void build_rmq() {
    int n = lcp.size();

    lg.assign(n + 1, 0);

    for (int i = 2; i <= n; ++i) {
        lg[i] = lg[i / 2] + 1;
    }

    st.assign(lg[n] + 1, vector<int>(n));
    st[0] = lcp;

    for (int k = 1; k < (int)st.size(); ++k) {
        int len = 1 << k;
        int half = len >> 1;

        for (int i = 0; i + len <= n; ++i) {
            st[k][i] = min(st[k - 1][i], st[k - 1][i + half]);
        }
    }
}

int get_lcp(int i, int j) {
    if (i == j) return (int)s.size() - sa[i];

    if (i > j) swap(i, j);

    ++i;

    int len = j - i + 1;
    int k = lg[len];

    return min(st[k][i], st[k][j - (1 << k) + 1]);
}

pair<int, int> cmp_suffix(int pos, const string& p, int known) {
    int n = s.size();
    int m = p.size();
    int i = known;

    while (i < m && pos + i < n && s[pos + i] == p[i]) {
        ++i;
    }

    if (i == m && pos + i == n) return {0, i};
    if (i == m) return {1, i};
    if (pos + i == n) return {-1, i};

    return {(unsigned char)s[pos + i] < (unsigned char)p[i] ? -1 : 1, i};
}

int lower_bound_lcp(const string& p) {
    int n = sa.size();

    auto lc = cmp_suffix(sa[0], p, 0);
    if (lc.first >= 0) return 0;

    auto rc = cmp_suffix(sa[n - 1], p, 0);
    if (rc.first < 0) return n;

    int l = 0;
    int r = n - 1;
    int lcp_l = lc.second;
    int lcp_r = rc.second;

    while (r - l > 1) {
        int m = (l + r) / 2;

        if (lcp_l >= lcp_r) {
            int x = get_lcp(l, m);

            if (x > lcp_l) {
                l = m;
            } else if (x < lcp_l) {
                r = m;
                lcp_r = x;
            } else {
                auto c = cmp_suffix(sa[m], p, lcp_l);

                if (c.first < 0) {
                    l = m;
                    lcp_l = c.second;
                } else {
                    r = m;
                    lcp_r = c.second;
                }
            }
        } else {
            int x = get_lcp(m, r);

            if (x > lcp_r) {
                r = m;
            } else if (x < lcp_r) {
                l = m;
                lcp_l = x;
            } else {
                auto c = cmp_suffix(sa[m], p, lcp_r);

                if (c.first < 0) {
                    l = m;
                    lcp_l = c.second;
                } else {
                    r = m;
                    lcp_r = c.second;
                }
            }
        }
    }

    return r;
}

bool next_key(const string& p, string& q) {
    q = p;

    for (int i = (int)q.size() - 1; i >= 0; --i) {
        unsigned char c = q[i];

        if (c < 255) {
            q[i] = char(c + 1);
            q.resize(i + 1);
            return true;
        }
    }

    return false;
}

pair<int, int> search_range(const string& p) {
    int first = lower_bound_lcp(p);

    string q;
    int last = next_key(p, q) ? lower_bound_lcp(q) : (int)sa.size();

    return {first, last};
}

void prepare(const string& text) {
    s = text;
    sa = build_sa(s);
    build_lcp();
    build_rmq();
}

double median(vector<double>& values) {
    sort(values.begin(), values.end());

    int n = values.size();

    if (n % 2) return values[n / 2];

    return (values[n / 2 - 1] + values[n / 2]) / 2.0;
}

string unary_string(int n) {
    return string(n, 'a');
}

vector<string> make_unary_patterns(int count, int len) {
    vector<string> patterns;
    patterns.reserve(count);

    string p(len, 'a');

    for (int i = 0; i < count; ++i) {
        patterns.push_back(p);
    }

    return patterns;
}

double measure_search(const vector<string>& patterns) {
    auto start = chrono::high_resolution_clock::now();

    size_t checksum = 0;

    for (const string& p : patterns) {
        auto range = search_range(p);
        checksum += range.second - range.first;
    }

    global_checksum += checksum;

    auto finish = chrono::high_resolution_clock::now();

    return chrono::duration<double, milli>(finish - start).count();
}

void benchmark_by_n() {
    int start_n = 10000;
    int levels = 8;
    int repeats = 5;
    int query_count = 10000;
    int m = 64;

    cout << "\nBenchmark 1: fixed m, growing n\n";
    cout << "Expected search complexity: O(m + log n), m is fixed\n\n";

    cout << left
         << setw(12) << "n"
         << setw(14) << "build_ms"
         << setw(14) << "search_ms"
         << setw(14) << "us/query"
         << setw(14) << "log2(n)"
         << setw(16) << "m+log2(n)"
         << setw(18) << "us/(m+logn)"
         << '\n';

    cout << string(102, '-') << '\n';

    for (int level = 0; level < levels; ++level) {
        int n = start_n * (1 << level);

        vector<double> build_times;
        vector<double> search_times;

        for (int rep = 0; rep < repeats; ++rep) {
            string text = unary_string(n);
            vector<string> patterns = make_unary_patterns(query_count, m);

            auto build_start = chrono::high_resolution_clock::now();

            prepare(text);

            auto build_finish = chrono::high_resolution_clock::now();

            double build_ms = chrono::duration<double, milli>(
                build_finish - build_start
            ).count();

            double search_ms = measure_search(patterns);

            build_times.push_back(build_ms);
            search_times.push_back(search_ms);
        }

        double build_ms = median(build_times);
        double search_ms = median(search_times);
        double us_per_query = search_ms * 1000.0 / query_count;
        double logn = log2((double)n);
        double expected = m + logn;

        cout << left
             << setw(12) << n
             << setw(14) << fixed << setprecision(3) << build_ms
             << setw(14) << fixed << setprecision(3) << search_ms
             << setw(14) << fixed << setprecision(6) << us_per_query
             << setw(14) << fixed << setprecision(3) << logn
             << setw(16) << fixed << setprecision(3) << expected
             << setw(18) << fixed << setprecision(8) << us_per_query / expected
             << '\n';
    }
}

void benchmark_by_m() {
    int n = 300000;
    int start_m = 4;
    int levels = 9;
    int repeats = 5;
    int query_count = 10000;

    string text = unary_string(n);
    prepare(text);

    cout << "\nBenchmark 2: fixed n, growing m\n";
    cout << "Expected search complexity: O(m + log n), n is fixed\n\n";

    cout << left
         << setw(12) << "m"
         << setw(14) << "search_ms"
         << setw(14) << "us/query"
         << setw(14) << "log2(n)"
         << setw(16) << "m+log2(n)"
         << setw(18) << "us/(m+logn)"
         << '\n';

    cout << string(88, '-') << '\n';

    for (int level = 0; level < levels; ++level) {
        int m = start_m * (1 << level);

        if (m > n) break;

        vector<string> patterns = make_unary_patterns(query_count, m);
        vector<double> search_times;

        for (int rep = 0; rep < repeats; ++rep) {
            double search_ms = measure_search(patterns);
            search_times.push_back(search_ms);
        }

        double search_ms = median(search_times);
        double us_per_query = search_ms * 1000.0 / query_count;
        double logn = log2((double)n);
        double expected = m + logn;

        cout << left
             << setw(12) << m
             << setw(14) << fixed << setprecision(3) << search_ms
             << setw(14) << fixed << setprecision(6) << us_per_query
             << setw(14) << fixed << setprecision(3) << logn
             << setw(16) << fixed << setprecision(3) << expected
             << setw(18) << fixed << setprecision(8) << us_per_query / expected
             << '\n';
    }
}

int main(int argc, char* argv[]) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int mode = 0;

    if (argc >= 2) {
        mode = stoi(argv[1]);
    }

    cout << "Suffix array + LCP + RMQ benchmark\n";
    cout << "mode 0: run both benchmarks\n";
    cout << "mode 1: fixed m, growing n\n";
    cout << "mode 2: fixed n, growing m\n";

    if (mode == 0 || mode == 1) {
        benchmark_by_n();
    }

    if (mode == 0 || mode == 2) {
        benchmark_by_m();
    }

    cerr << "checksum: " << global_checksum << '\n';

    return 0;
}