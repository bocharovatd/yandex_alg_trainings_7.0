// G. Острова

// Одно разбросанное на островах Океании государство решило создать сеть автомобильных дорог (вернее, мостов). 
// По каждому мосту можно перемещаться в обе стороны. Был разработан план очередности строительства мостов, 
// и известно, что после постройки всех мостов можно будет проехать по ним с каждого острова на каждый 
// (возможно, через некоторые промежуточные острова).

// Однако этот момент может наступить до того, как будут построены все мосты. 
// Вам необходимо определить такое минимальное количество мостов, после строительства которых 
// (в порядке, определенном планом) можно будет попасть с любого острова на любой другой.

// Формат ввода
// Первая строка содержит два числа: число островов N (1≤N≤10000) и количество мостов в плане M (1≤M≤50000). 
// Далее идёт M строк, каждая содержит два числа x и y (1≤x,y≤N) — номера островов, которые соединяет очередной мост в плане.

// Формат вывода
// Программа должна вывести единственное число — минимальное количество построенных мостов, 
// после которого можно будет попасть с любого острова на любой другой.

#include <iostream>
#include <vector>

class DisjoinedSets {
public:
    DisjoinedSets(int n);
    ~DisjoinedSets();
    void Join(int firstI, int secondI);
    int Count();
private:
    struct Node {
        bool isRoot;
        Node* next;
    };
    std::vector<Node*> disjoinedSets;
    int countRoots;
    void join(Node* firstNode, Node* secondNode);
    std::pair<Node*, int> findRootAndDepth(Node* node);
};

DisjoinedSets::DisjoinedSets(int n) {
    countRoots = 0;
    disjoinedSets.resize(n);
    for (int i = 0; i < n; ++i) {
        Node* node = new Node;
        node->isRoot = true;
        node->next = node;
        disjoinedSets[i] = node;
        ++countRoots;
    }
}

DisjoinedSets::~DisjoinedSets() {
    for (auto node: disjoinedSets) {
        delete node;
    }
}

void DisjoinedSets::Join(int firstI, int secondI) {
    return join(disjoinedSets[firstI - 1], disjoinedSets[secondI - 1]);
}

void DisjoinedSets::join(Node* firstNode, Node* secondNode) {
    std::pair<Node*, int> firstRootDepth = findRootAndDepth(firstNode);
    Node* firstRoot = firstRootDepth.first;
    int firstDepth = firstRootDepth.second;
    std::pair<Node*, int> secondRootDepth = findRootAndDepth(secondNode);
    Node* secondRoot = secondRootDepth.first;
    int secondDepth = secondRootDepth.second;
    if (firstRoot != secondRoot) {
        if (firstDepth < secondDepth) {
            firstRoot->isRoot = false;
            firstRoot->next = secondRoot;
        } else {
            secondRoot->isRoot = false;
            secondRoot->next = firstRoot;
        }
        --countRoots;
    }
}

std::pair<DisjoinedSets::Node*, int> DisjoinedSets::findRootAndDepth(Node* node) {
    int depth = 0;
    Node* curNode = node;
    while (!curNode->isRoot) {
        curNode = curNode->next;
        ++depth;
    }
    return std::make_pair(curNode, depth);
}

int DisjoinedSets::Count() {
    return countRoots;
}

int main() {
    int n;
    std::cin >> n;
    DisjoinedSets disjoinedSets(n);
    int countBridges;
    std::cin >> countBridges;
    bool flagResultFound = false;
    int resultI = 0;
    for (int i = 0; i < countBridges; ++i) {
        int firstI, secondI;
        std::cin >> firstI >> secondI;
        if (!flagResultFound) {
            disjoinedSets.Join(firstI, secondI);
            if (disjoinedSets.Count() == 1) {
                flagResultFound = true;
                resultI = i + 1;
            }
        }
    }
    std::cout << resultI << std::endl;
    return 0;
}
