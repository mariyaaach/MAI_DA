#include <cstdio>
#include <algorithm>
#include <climits>
#include <vector>
#include <queue>
#include <cmath>

using namespace std;

typedef long long LL;

const int MAX_N = 100005;
const int MAX_D = 10;

int d; // Размерность

struct Point {
    int coord[MAX_D];
    int index; // Индексация с 1
};

struct KDNode {
    int point_idx; // Индекс точки в массиве точек
    int left, right;
    int min_coord[MAX_D], max_coord[MAX_D]; // MBB для ускорения поиска
};

Point points[MAX_N];
KDNode tree[MAX_N];
int nodeCount;

void updateBoundingBox(int node_idx) {
    KDNode &node = tree[node_idx];
    for (int i = 0; i < d; ++i) {
        node.min_coord[i] = node.max_coord[i] = points[node.point_idx].coord[i];
        if (node.left != -1) {
            node.min_coord[i] = min(node.min_coord[i], tree[node.left].min_coord[i]);
            node.max_coord[i] = max(node.max_coord[i], tree[node.left].max_coord[i]);
        }
        if (node.right != -1) {
            node.min_coord[i] = min(node.min_coord[i], tree[node.right].min_coord[i]);
            node.max_coord[i] = max(node.max_coord[i], tree[node.right].max_coord[i]);
        }
    }
}

int build(int l, int r, int depth) {
    if (l > r) return -1;
    int axis = depth % d;
    int m = (l + r) / 2;

    nth_element(points + l, points + m, points + r + 1, [axis](const Point &a, const Point &b) {
        if (a.coord[axis] != b.coord[axis])
            return a.coord[axis] < b.coord[axis];
        else
            return a.index < b.index;
    });

    int idx = nodeCount++;
    tree[idx].point_idx = m;
    tree[idx].left = build(l, m - 1, depth + 1);
    tree[idx].right = build(m + 1, r, depth + 1);

    updateBoundingBox(idx);

    return idx;
}

LL squaredDistance(const Point &a, const Point &b) {
    LL dist = 0;
    for (int i = 0; i < d; ++i) {
        LL diff = (LL)a.coord[i] - b.coord[i];
        dist += diff * diff;
    }
    return dist;
}

LL minDistanceToBox(const Point &p, int node_idx) {
    LL dist = 0;
    for (int i = 0; i < d; ++i) {
        if (p.coord[i] < tree[node_idx].min_coord[i]) {
            LL diff = (LL)p.coord[i] - tree[node_idx].min_coord[i];
            dist += diff * diff;
        } else if (p.coord[i] > tree[node_idx].max_coord[i]) {
            LL diff = (LL)p.coord[i] - tree[node_idx].max_coord[i];
            dist += diff * diff;
        }
    }
    return dist;
}

struct QueueNode {
    int node_idx;
    LL dist;

    bool operator<(const QueueNode &other) const {
        return dist > other.dist; // Для минимальной кучи
    }
};

int nearest(const Point &target) {
    priority_queue<QueueNode> pq;
    pq.push({0, minDistanceToBox(target, 0)});

    LL bestDist = LLONG_MAX;
    int bestIndex = -1;

    while (!pq.empty()) {
        QueueNode qn = pq.top();
        pq.pop();

        if (qn.dist >= bestDist) continue;

        int idx = qn.node_idx;
        LL nodeDist = squaredDistance(points[tree[idx].point_idx], target);
        if (nodeDist < bestDist || (nodeDist == bestDist && points[tree[idx].point_idx].index < bestIndex)) {
            bestDist = nodeDist;
            bestIndex = points[tree[idx].point_idx].index;
        }

        if (tree[idx].left != -1) {
            LL distLeft = minDistanceToBox(target, tree[idx].left);
            if (distLeft < bestDist) {
                pq.push({tree[idx].left, distLeft});
            }
        }
        if (tree[idx].right != -1) {
            LL distRight = minDistanceToBox(target, tree[idx].right);
            if (distRight < bestDist) {
                pq.push({tree[idx].right, distRight});
            }
        }
    }

    return bestIndex;
}

int readInt() {
    int x = 0, c = getchar_unlocked(), sign = 1;
    while (c != '-' && (c < '0' || c > '9')) c = getchar_unlocked();
    if (c == '-') sign = -1, c = getchar_unlocked();
    while (c >= '0' && c <= '9') x = x * 10 + c - '0', c = getchar_unlocked();
    return x * sign;
}

void writeInt(int x) {
    char buf[12];
    int i = 10;
    buf[11] = '\n';
    if (x == 0) {
        putchar_unlocked('0');
        putchar_unlocked('\n');
        return;
    }
    bool neg = x < 0;
    if (neg) x = -x;
    while (x > 0) {
        buf[i--] = x % 10 + '0';
        x /= 10;
    }
    if (neg) buf[i--] = '-';
    for (++i; i <= 11; ++i) putchar_unlocked(buf[i]);
}

int main() {
    int n;
    n = readInt();
    d = readInt();
    for (int i = 0; i < n; ++i) {
        Point &p = points[i];
        for (int j = 0; j < d; ++j) {
            p.coord[j] = readInt();
        }
        p.index = i + 1; // Индексация с 1
    }
    nodeCount = 0;
    build(0, n - 1, 0);

    int q;
    q = readInt();
    Point queryPoint;
    for (int i = 0; i < q; ++i) {
        for (int j = 0; j < d; ++j) {
            queryPoint.coord[j] = readInt();
        }
        int bestIndex = nearest(queryPoint);
        writeInt(bestIndex);
    }
    return 0;
}
