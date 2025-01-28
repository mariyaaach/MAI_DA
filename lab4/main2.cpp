#include <iostream>
#include <vector>
#include <string>

using namespace std;

vector<int> calculateZ(const string &S) {
    int n = S.size();
    vector<int> Z(n, 0);
    int L = 0, R = 0;

    for (int i = 1; i < n; i++) {
        if (i <= R)
            Z[i] = min(R - i + 1, Z[i - L]);
        while (i + Z[i] < n && S[Z[i]] == S[i + Z[i]])
            ++Z[i];
        if (i + Z[i] - 1 > R) {
            L = i;
            R = i + Z[i] - 1;
        }
    }
    return Z;
}

int main() {
    string text;
    string pattern;
    getline(cin, text);
    getline(cin, pattern);

    string concatenated = pattern + "#" + text;

    vector<int> Z = calculateZ(concatenated);
    int pattern_length = pattern.size();
    int concatenated_length = concatenated.size();

    for (int i = pattern_length + 1; i < concatenated_length; ++i) {
        if (Z[i] == pattern_length) {
            cout << (i - pattern_length - 1) << endl;  
        }
    }

    return 0;
}
