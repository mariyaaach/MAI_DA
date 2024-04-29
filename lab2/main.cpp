#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cstdint>
#include <limits>

using namespace std;

struct TreeNode {
    string identifier;
    int priority; 
    uint64_t data_value;
    TreeNode *left_node, *right_node;

    // Конструктор с инициализацией списка
    TreeNode(const string& id, uint64_t data)
        : identifier(id), data_value(data), priority(rand()),
          left_node(nullptr), right_node(nullptr) {}

    ~TreeNode() = default; 
};

// Преобразование строки в нижний регистр
void toLowerCase(string& s) {
    for (char& c : s) {
        c = tolower(c);
    }
}

// Разделение дерева на два поддерева по заданному ключу
void split(TreeNode *current, TreeNode *&leftTree, TreeNode *&rightTree, const string& pivot) {
    if (current == nullptr) {
        leftTree = rightTree = nullptr;
        return;
    }

    if (current->identifier < pivot) {
        leftTree = current;
        split(current->right_node, current->right_node, rightTree, pivot);
    } else {
        rightTree = current;
        split(current->left_node, leftTree, current->left_node, pivot);
    }
}

// Функция для добавления нового элемента в дерево
void insert(TreeNode *&root, TreeNode *newItem) {
    if (root == nullptr) {
        root = newItem;
        return;
    }

    if (newItem->priority > root->priority) {
        split(root, newItem->left_node, newItem->right_node, newItem->identifier);
        root = newItem;
    } else if (newItem->identifier < root->identifier) {
        insert(root->left_node, newItem);
    } else {
        insert(root->right_node, newItem);
    }
}

// Комбинирует два поддерева в одно
TreeNode* merge(TreeNode *left, TreeNode *right) {
    if (!left) return right;
    if (!right) return left;

    if (left->priority > right->priority) {
        left->right_node = merge(left->right_node, right);
        return left;
    } else {
        right->left_node = merge(left, right->left_node);
        return right;
    }
}

// Удаление элемента по ключу
void remove(TreeNode *&root, const string& id) {
    if (!root) return;

    if (root->identifier == id) {
        TreeNode *merged = merge(root->left_node, root->right_node);
        delete root;
        root = merged;
    } else {
        remove((id < root->identifier) ? root->left_node : root->right_node, id);
    }
}

// Поиск элемента по ключу
TreeNode* find(TreeNode *root, const string& id) {
    while (root) {
        if (root->identifier == id) {
            return root;
        }
        root = (id < root->identifier) ? root->left_node : root->right_node;
    }
    return nullptr;
}

// Освобождение памяти всего дерева
void clear(TreeNode *root) {
    if (root) {
        clear(root->left_node);
        clear(root->right_node);
        delete root;
    }
}

int main() {
    srand(time(nullptr));
    TreeNode *root = nullptr;

    string action;
    string identifier;
    uint64_t data;

    while (cin >> action) {
        if (action == "+") {
            cin >> identifier >> data;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            toLowerCase(identifier);
            if (find(root, identifier)) {
                cout << "Exist\n";
            } else {
                insert(root, new TreeNode(identifier, data));
                cout << "OK\n";
            }
        } else if (action == "-") {
            cin >> identifier;
            toLowerCase(identifier);
            if (find(root, identifier)) {
                remove(root, identifier);
                cout << "OK\n";
            } else {
                cout << "NoSuchWord\n";
            }
        } else {
            toLowerCase(action);
            TreeNode *foundNode = find(root, action);
            if (foundNode) {
                cout << "OK: " << foundNode->data_value << "\n";
            } else {
                cout << "NoSuchWord\n";
            }
        }
    }

    clear(root);
}