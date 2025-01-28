#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cctype>

using namespace std;

// Функция для вычисления Z-функции строки S
vector<int> calculateZ(const string &S) {
    int n = S.size();
    vector<int> Z(n);
    int L = 0, R = 0;

    for (int i = 1; i < n; i++) {
        if (i <= R)
            Z[i] = min(R - i + 1, Z[i - L]);
        while (i + Z[i] < n && tolower(S[Z[i]]) == tolower(S[i + Z[i]]))
            ++Z[i];
        if (i + Z[i] - 1 > R) {
            L = i;
            R = i + Z[i] - 1;
        }
    }
    return Z;
}

int main() {
    string pattern, temp;
    getline(cin, pattern);

    string concatenated = pattern + "#";

    // Считываем текст как отдельные строки
    vector<string> text_lines;
    while (getline(cin, temp) && !temp.empty()) {
        text_lines.push_back(temp);
    }

    // Конкатенация образца и текста
    for (const auto &line : text_lines) {
        concatenated += line + " ";
    }

    vector<int> Z = calculateZ(concatenated);
    int pattern_length = pattern.size();

    // Проход по тексту и вывод найденных вхождений
    int line_count = 1, word_count = 1;
    int start_text_index = pattern_length; // Изменено на pattern_length

    for (int i = start_text_index; i < concatenated.size(); ++i) {
        if (Z[i] == pattern_length) {
            // Посчитаем позицию вхождения слова в тексте
            cout << line_count << "," << word_count << endl;
        }

        if (concatenated[i] == ' ') {
            word_count++;
        }
        if (concatenated[i] == '\n') {
            line_count++;
            word_count = 1;
        }
    }

    return 0;
}
