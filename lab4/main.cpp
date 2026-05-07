#include <bits/stdc++.h>
using namespace std;

struct Position {
    int line;
    int word;
};

string normalizeWord(string s) {
    for (char &c : s) {
        if ('A' <= c && c <= 'Z') {
            c = char(c - 'A' + 'a');
        }
    }
    return s;
}

vector<int> splitLineToIds(
    const string &line,
    unordered_map<string, int> &ids
) {
    vector<int> result;
    stringstream ss(line);
    string word;

    while (ss >> word) {
        word = normalizeWord(word);

        auto it = ids.find(word);
        if (it == ids.end()) {
            int newId = (int)ids.size();
            ids[word] = newId;
            result.push_back(newId);
        } else {
            result.push_back(it->second);
        }
    }

    return result;
}

// suff[i] = длина наибольшего суффикса P[0..i],
// который совпадает с суффиксом всего образца P.
vector<int> buildSuffixes(const vector<int> &pattern) {
    int m = (int)pattern.size();
    vector<int> suff(m, 0);

    if (m == 0) {
        return suff;
    }

    suff[m - 1] = m;

    int g = m - 1;
    int f = 0;

    for (int i = m - 2; i >= 0; --i) {
        if (i > g && suff[i + m - 1 - f] < i - g) {
            suff[i] = suff[i + m - 1 - f];
        } else {
            if (i < g) {
                g = i;
            }

            f = i;

            while (g >= 0 && pattern[g] == pattern[g + m - 1 - f]) {
                --g;
            }

            suff[i] = f - g;
        }
    }

    return suff;
}

// Таблица хорошего суффикса для Бойера-Мура.
// goodSuffix[i] = сдвиг при несовпадении в позиции i образца.
vector<int> buildGoodSuffix(const vector<int> &pattern) {
    int m = (int)pattern.size();
    vector<int> goodSuffix(m, m);
    vector<int> suff = buildSuffixes(pattern);

    int j = 0;

    for (int i = m - 1; i >= -1; --i) {
        if (i == -1 || suff[i] == i + 1) {
            while (j < m - 1 - i) {
                if (goodSuffix[j] == m) {
                    goodSuffix[j] = m - 1 - i;
                }
                ++j;
            }
        }
    }

    for (int i = 0; i <= m - 2; ++i) {
        goodSuffix[m - 1 - suff[i]] = m - 1 - i;
    }

    return goodSuffix;
}

vector<int> apostolicoGiancarlo(
    const vector<int> &pattern,
    const vector<int> &text
) {
    vector<int> occurrences;

    int m = (int)pattern.size();
    int n = (int)text.size();

    if (m == 0 || n < m) {
        return occurrences;
    }

    vector<int> suff = buildSuffixes(pattern);
    vector<int> goodSuffix = buildGoodSuffix(pattern);

    // last[x] = последнее вхождение слова x в образце.
    unordered_map<int, int> last;
    last.reserve(m * 2 + 1);

    for (int i = 0; i < m; ++i) {
        last[pattern[i]] = i;
    }

    // skip[pos] хранит, сколько правых слов образца уже совпадало
    // при сравнении, которое заканчивалось в позиции pos текста.
    vector<int> skip(n, 0);

    int shift = 0;

    while (shift <= n - m) {
        int i = m - 1;

        while (i >= 0) {
            int textPos = shift + i;
            int known = skip[textPos];

            if (known == 0) {
                if (pattern[i] == text[textPos]) {
                    --i;
                } else {
                    break;
                }
            } else {
                // Нам не нужно использовать больше символов, чем осталось слева.
                known = min(known, i + 1);

                // Если соответствующий кусок образца совпадает с суффиксом образца,
                // можно не сравнивать эти known слов повторно.
                if (suff[i] >= known) {
                    i -= known;
                } else {
                    // Иначе мы знаем, что внутри этого блока есть несовпадение.
                    i -= suff[i];
                    break;
                }
            }
        }

        int matched = m - 1 - i;
        int endPos = shift + m - 1;

        if (i < 0) {
            occurrences.push_back(shift);

            skip[endPos] = m;

            // Полное совпадение: сдвигаемся по правилу хорошего суффикса.
            shift += goodSuffix[0];
        } else {
            skip[endPos] = matched;

            int badCharShift;

            auto it = last.find(text[shift + i]);
            if (it == last.end()) {
                badCharShift = i + 1;
            } else {
                badCharShift = i - it->second;
                if (badCharShift < 1) {
                    badCharShift = 1;
                }
            }

            int currentShift = max(goodSuffix[i], badCharShift);

            if (currentShift < 1) {
                currentShift = 1;
            }

            shift += currentShift;
        }
    }

    return occurrences;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    unordered_map<string, int> ids;
    ids.reserve(1 << 20);
    ids.max_load_factor(0.7);

    string patternLine;

    if (!getline(cin, patternLine)) {
        return 0;
    }

    vector<int> pattern = splitLineToIds(patternLine, ids);

    vector<int> text;
    vector<Position> positions;

    string line;
    int lineNumber = 0;

    while (getline(cin, line)) {
        ++lineNumber;

        stringstream ss(line);
        string word;
        int wordNumber = 0;

        while (ss >> word) {
            ++wordNumber;

            word = normalizeWord(word);

            auto it = ids.find(word);
            int id;

            if (it == ids.end()) {
                id = (int)ids.size();
                ids[word] = id;
            } else {
                id = it->second;
            }

            text.push_back(id);
            positions.push_back({lineNumber, wordNumber});
        }
    }

    vector<int> occurrences = apostolicoGiancarlo(pattern, text);

    for (int startIndex : occurrences) {
        cout << positions[startIndex].line << ", "
             << positions[startIndex].word << '\n';
    }

    return 0;
}