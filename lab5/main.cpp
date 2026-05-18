#include <bits/stdc++.h>
using namespace std;

string s;
vector<int> sa, rnk, lcp, lg;
vector<vector<int>> st;

void trim_cr(string& x) {
    if (!x.empty() && x.back() == '\r') x.pop_back();
}

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
            pair<int, int> cur = {c[p[i]], c[(p[i] + len) % n]};
            pair<int, int> prv = {c[p[i - 1]], c[(p[i - 1] + len) % n]};

            if (cur != prv) ++ncls;
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

void add_int(string& out, int x) {
    char buf[16];
    int len = 0;

    if (x == 0) {
        buf[len++] = '0';
    } else {
        char rev[16];
        int cnt = 0;

        while (x) {
            rev[cnt++] = char('0' + x % 10);
            x /= 10;
        }

        while (cnt) {
            buf[len++] = rev[--cnt];
        }
    }

    out.append(buf, buf + len);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (!getline(cin, s)) return 0;
    trim_cr(s);

    if (s.empty()) return 0;

    sa = build_sa(s);
    build_lcp();
    build_rmq();

    string p;
    vector<int> ans;
    string out;

    out.reserve(1 << 20);

    for (int num = 1; getline(cin, p); ++num) {
        trim_cr(p);

        if (p.empty() || p.size() > s.size()) continue;

        int first = lower_bound_lcp(p);

        string q;
        int last = next_key(p, q) ? lower_bound_lcp(q) : (int)sa.size();

        if (first >= last) continue;

        ans.clear();
        ans.reserve(last - first);

        for (int i = first; i < last; ++i) {
            ans.push_back(sa[i] + 1);
        }

        sort(ans.begin(), ans.end());

        add_int(out, num);
        out += ": ";

        for (int i = 0; i < (int)ans.size(); ++i) {
            if (i) out += ", ";
            add_int(out, ans[i]);
        }

        out += '\n';

        if (out.size() > (1 << 20)) {
            cout << out;
            out.clear();
        }
    }

    cout << out;

    return 0;
}