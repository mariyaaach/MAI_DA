#include <iostream>
#include <map>
#include <vector>
#include <string>

using namespace std;

class SuffixTree {
private:
    const static int MAX_LENGTH = 1e9; // Значение для максимальной длины ребра
    const static int TAB_SIZE = 4;     // Размер табуляции для вывода дерева

    struct TreeNode {
        map<char, int> transitions; // Переходы из узла
        int start, end;                  // Начало и конец ребра
        int suffixLink = -1;             // Суффиксная ссылка

        TreeNode(int _start, int _end) : start(_start), end(_end) {}
    };

    using Edge = pair<char, int>; // Тип для ребра

    // Проверяет, есть ли переход по символу c из узла node
    bool hasTransition(int node, char c) {
        return nodes[node].transitions.count(c) > 0;
    }

    // Получает узел, на который ведет переход по символу c из узла node
    int getTransition(int node, char c) {
        return nodes[node].transitions[c];
    }

    string input;            // Исходная строка
    vector<TreeNode> nodes; // Вектор узлов дерева
    int textLength;              // Текущая длина строки
    int nodeCount;              // Счетчик узлов
    int activeNode = 0;        // Активный узел
    int activeLength = 0;      // Длина активного ребра
    int remaining = 0;         // Количество оставшихся суффиксов для обработки

    void traverseEdge();        // Проходит по активному ребру
    int createNewNode(int start = 0, int end = MAX_LENGTH); // Создает новый узел
    void construct();           // Строит дерево
    void addCharacter(char c); // Добавляет новый символ
    void outputEdge(int node, std::ostream & out); // Выводит ребро узла
    void outputTree(int node, std::ostream & out, int depth); // Выводит дерево

public:
    SuffixTree(const string & str); 
    friend ostream & operator << (ostream & out, SuffixTree & st); // Оператор вывода
    string findLexicographicallyMinimal(int length); // Находит минимальный лексикографический разрез
};

void SuffixTree::traverseEdge() {
    while (true) {
        char c = input[textLength - remaining];
        if (!hasTransition(activeNode, c)) {
            return;
        }
        int nextNode = getTransition(activeNode, c);
        if (remaining > nodes[nextNode].end) {
            remaining -= nodes[nextNode].end;
            activeNode = nextNode;
        } else {
            return;
        }
    }
}

int SuffixTree::createNewNode(int start, int end) {
    nodes.emplace_back(start, end);
    return ++nodeCount;
}

void SuffixTree::construct() {
    textLength = 0;
    nodeCount = 0;
    for (char c : input) {
        addCharacter(c);
    }
}

void SuffixTree::addCharacter(char c) {
    ++textLength;
    ++remaining;
    int lastNode = 0;
    while (remaining > 0) {
        traverseEdge();
        char edgeChar = input[textLength - remaining];
        if (!hasTransition(activeNode, edgeChar)) {
            // Создаем новый лист, если нет перехода
            nodes[activeNode].transitions[edgeChar] = createNewNode(textLength - remaining);
            nodes[lastNode].suffixLink = activeNode;
            lastNode = activeNode;
        } else {
            int nextNode = getTransition(activeNode, edgeChar);
            int edgeLength = remaining - 1;
            char nextChar = input[nodes[nextNode].start + edgeLength];
            if (nextChar != c) {
                // Разделяем ребро, если символы не совпадают
                int splitNode = createNewNode(nodes[nextNode].start, edgeLength);
                int newLeaf = createNewNode(textLength - 1, MAX_LENGTH);
                nodes[nextNode].start += edgeLength;
                nodes[nextNode].end -= edgeLength;
                nodes[activeNode].transitions[edgeChar] = splitNode;
                nodes[splitNode].transitions[nextChar] = nextNode;
                nodes[splitNode].transitions[c] = newLeaf;
                nodes[lastNode].suffixLink = splitNode;
                lastNode = splitNode;
            } else {
                // Завершаем фазу, если символы совпадают
                nodes[lastNode].suffixLink = activeNode;
                return;
            }
        }
        if (activeNode == 0) {
            --remaining;
        } else {
            activeNode = nodes[activeNode].suffixLink;
        }
    }
}

void SuffixTree::outputEdge(int node, std::ostream & out) {
    int edgeLength = std::min(textLength - nodes[node].start, nodes[node].end);
    for (int i = 0; i < edgeLength; ++i) {
        out << input[nodes[node].start + i];
    }
}

void SuffixTree::outputTree(int node, std::ostream & out, int depth) {
    for (const Edge & edge : nodes[node].transitions) {
        for (int i = 0; i < TAB_SIZE * (depth - 1); ++i) {
            out << ' ';
        }
        int nextNode = edge.second;
        out << "|-> {id = " << nextNode << ", ";
        outputEdge(nextNode, out);
        out << ", suffixLink = " << nodes[nextNode].suffixLink << "}\n";
        outputTree(nextNode, out, depth + 1);
    }
}

SuffixTree::SuffixTree(const std::string & str) : input(str) {
    createNewNode(0, MAX_LENGTH);
    construct();
}

ostream & operator << (std::ostream & out, SuffixTree & st) {
    out << "root\n";
    st.outputTree(0, out, 1);
    return out;
}

string SuffixTree::findLexicographicallyMinimal(int length) {
    int currentNode = nodes[0].transitions.begin()->second;
    int pos = 0;
    string result;
    while (length > 0) {
        if (pos == nodes[currentNode].end) {
            currentNode = nodes[currentNode].transitions.begin()->second;
            pos = 0;
        } else {
            result.push_back(input[nodes[currentNode].start + pos]);
            ++pos;
            --length;
        }
    }
    return result;
}

int main() {

	std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);

    string s;
    cin >> s;
    string doubledString = s + s;
    doubledString.push_back('z' + 1); // Добавляем символ больше любого в исходной строке
    SuffixTree st(doubledString);
    cout << st.findLexicographicallyMinimal(s.size()) << endl;

    return 0;
}
