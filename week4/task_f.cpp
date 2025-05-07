// F. Свинки-копилки

// У Васи есть N свинок-копилок, свинки занумерованы числами от 1 до N. 
// Каждая копилка может быть открыта единственным соответствующим ей ключом или разбита.

// Вася положил ключи в некоторые из копилок (он помнит, какой ключ лежит в какой из копилок). 
// Теперь Вася собрался купить машину, а для этого ему нужно достать деньги из всех копилок. 
// При этом он хочет разбить как можно меньшее количество копилок 
// (ведь ему ещё нужно копить деньги на квартиру, дачу, вертолет…). 
// Помогите Васе определить, какое минимальное количество копилок нужно разбить.

// Формат ввода
// В первой строке содержится число N — количество свинок-копилок (1 ≤ N ≤ 100000). 
// Далее идет N строк с описанием того, где лежит ключ от какой копилки: 
// в i-ой из этих строк записан номер копилки, в которой находится ключ от i-ой копилки.

// Формат вывода
// Выведите единственное число: минимальное количество копилок, которые необходимо разбить.

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
    void join(Node* firstNode, Node* secondNode);
    std::pair<Node*, int> findRootAndDepth(Node* node);
};

DisjoinedSets::DisjoinedSets(int n) {
    disjoinedSets.resize(n);
    for (int i = 0; i < n; ++i) {
        Node* node = new Node;
        node->isRoot = true;
        node->next = node;
        disjoinedSets[i] = node;
    }
}

DisjoinedSets::~DisjoinedSets() {
    for (auto node: disjoinedSets) {
        delete node;
    }
}

void DisjoinedSets::Join(int firstI, int secondI) {
    join(disjoinedSets[firstI - 1], disjoinedSets[secondI - 1]);
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
    int count = 0;
    for (auto node: disjoinedSets) {
        if (node->isRoot) {
            ++count;
        }
    }
    return count;
}

int main() {
    int n;
    std::cin >> n;
    DisjoinedSets disjoinedSets(n);
    for (int i = 1; i < n + 1; ++i) {
        int joinI;
        std::cin >> joinI;
        disjoinedSets.Join(i, joinI);
    }
    std::cout << disjoinedSets.Count();
    return 0;
}
