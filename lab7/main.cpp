#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    int n, m;
    cin >> n >> m;

    vector<int> w(n), c(n);
    for (int i = 0; i < n; i++) {
        cin >> w[i] >> c[i];
    }

    // dp[i][j] - это максимальная стоимость для i предметов и рюкзака вместимостью j
    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));

    // Сначала заполним динамическую таблицу
    for (int i = 1; i <= n; i++) {
        for (int j = 0; j <= m; j++) {
            // Если предмет не помещается в рюкзак
            dp[i][j] = dp[i - 1][j];
            // Если предмет помещается в рюкзак
            if (j >= w[i - 1]) {
                dp[i][j] = max(dp[i][j], dp[i - 1][j - w[i - 1]] + c[i - 1]);
            }
        }
    }

    // Теперь находим максимальную стоимость
    int maxValue = 0;
    for (int j = 0; j <= m; j++) {
        maxValue = max(maxValue, dp[n][j]);
    }

    // Выводим максимальную стоимость
    cout << maxValue << endl;

    // Составляем подмножество предметов для вывода
    vector<int> result;
    int remainingWeight = m;
    for (int i = n; i > 0; i--) {
        if (dp[i][remainingWeight] != dp[i - 1][remainingWeight]) {
            result.push_back(i);
            remainingWeight -= w[i - 1];
        }
    }

    // Выводим индексы предметов
    reverse(result.begin(), result.end());
    for (int i : result) {
        cout << i << " ";
    }
    cout << endl;

    return 0;
}
