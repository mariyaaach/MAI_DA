#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cctype>

using namespace std;

// Функция для приведения строки к нижнему регистру
string toLowerCase(const string& s) {
    string result;
    for (char c : s) {
        result += tolower(c);
    }
    return result;
}

// Функция для вычисления префикс-функции (массив pi)
vector<int> computePrefixFunction(const string &pattern) {
    int m = pattern.size();
    vector<int> pi(m, 0);
    int k = 0;
    for (int i = 1; i < m; ++i) {
        while (k > 0 && pattern[k] != pattern[i]) {
            k = pi[k - 1];
        }
        if (pattern[k] == pattern[i]) {
            ++k;
        }
        pi[i] = k;
    }
    return pi;
}

// Функция для поиска образца в тексте с использованием алгоритма КМП
void KMPSearch(const string &pattern, const vector<string> &text) {
    string pat = toLowerCase(pattern);
    vector<int> pi = computePrefixFunction(pat);
    int m = pat.size();
    int line_count = 1;

    for (const auto &line : text) {
        istringstream iss(line);
        string word;
        int word_count = 1;
        
        while (iss >> word) {
            string lower_word = toLowerCase(word);
            int n = lower_word.size();
            int k = 0;

            for (int i = 0; i < n; ++i) {
                while (k > 0 && pat[k] != lower_word[i]) {
                    k = pi[k - 1];
                }
                if (pat[k] == lower_word[i]) {
                    ++k;
                }
                if (k == m) {
                    cout << line_count << "," << word_count << endl;
                    k = pi[k - 1];
                    break;  // Переход к следующему слову после нахождения образца
                }
            }
            ++word_count;
        }
        ++line_count;
    }
}

int main() {
    string pattern;
    getline(cin, pattern);

    vector<string> text;
    string temp;
    while (getline(cin, temp)) {
        if (!temp.empty()) {
            text.push_back(temp);
        }
    }

    KMPSearch(pattern, text);

    return 0;
}
