// H. Разрезание графа

// Дан неориентированный граф. Над ним в заданном порядке производят операции следующих двух типов: 
// cut — разрезать граф, то есть удалить из него ребро; 
// ask — проверить, лежат ли две вершины графа в одной компоненте связности. 
// Известно, что после выполнения всех операций типа cut рёбер в графе не осталось. 
// Найдите результат выполнения каждой из операций типа ask.

// Формат ввода
// Первая строка входного файла содержит три целых числа, разделённые пробелами — количество вершин графа n, 
// количество рёбер m и количество операций k (1≤n≤50000, 0≤m≤100000, m≤k≤150000). 
// Следующие m строк задают рёбра графа; i-ая из этих строк содержит два числа u i и v i (1≤u i ,v i ≤n), 
// разделённые пробелами — номера концов i-го ребра. Далее следуют k строк, описывающих операции. 
// Операция типа cut задаётся строкой "cut u v" (1≤u,v≤n), которая означает, 
// что из графа удаляют ребро между вершинами u и v. Операция типа ask задаётся строкой "ask u v" (1≤u,v≤n), 
// которая означает, что необходимо узнать, лежат ли в данный момент вершины u и v в одной компоненте связности. 
// Гарантируется, что каждое ребро графа встретится в операциях типа cut ровно один раз.

// Формат вывода
// Для каждой операции ask во входном файле выведите на отдельной строке слово "YES", 
// если две указанные вершины лежат в одной компоненте связности, и "NO" в противном случае. 
// Порядок ответов должен соответствовать порядку операций ask во входном файле.

#include <iostream>
#include <vector>

class DisjoinedSets {
public:
    DisjoinedSets(int n);
    ~DisjoinedSets();
    void Join(int firstI, int secondI);
    void Disjoin(int firstI, int secondI);
    bool AreInOneSet(int firstI, int secondI);
private:
    struct Node {
        bool isRoot;
        Node* next;
    };
    std::vector<Node*> disjoinedSets;
    void join(Node* firstNode, Node* secondNode);
    void disjoin(Node* firstNode, Node* secondNode);
    bool areInOneSet(Node* firstNode, Node* secondNode);
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
    }
}

bool DisjoinedSets::AreInOneSet(int firstI, int secondI) {
    return areInOneSet(disjoinedSets[firstI - 1], disjoinedSets[secondI - 1]);
}

bool DisjoinedSets::areInOneSet(Node* firstNode, Node* secondNode) {
    std::pair<Node*, int> firstRootDepth = findRootAndDepth(firstNode);
    Node* firstRoot = firstRootDepth.first;
    std::pair<Node*, int> secondRootDepth = findRootAndDepth(secondNode);
    Node* secondRoot = secondRootDepth.first;
    if (firstRoot == secondRoot) {
        return true;
    }
    return false;
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

struct Operation {
    std::string title;
    int firstI;
    int secondI;
};

int main() {
    int countVerteces, countEdges, countOps;
    std::cin >> countVerteces >> countEdges >> countOps;
    DisjoinedSets disjoinedSets(countVerteces);
    std::vector<Operation> ops(countOps);
    std::vector<bool> askResults(countOps);
    int countAskResults = 0;
    for (int i = 0; i < countEdges; ++i) {
        int firstI, secondI;
        std::cin >> firstI >> secondI;
    }
    for (int i = 0; i < countOps; ++i) {
        std::cin >> ops[i].title >> ops[i].firstI >> ops[i].secondI;
        if (ops[i].title == "ask") {
            ++countAskResults;
        }
    }
    int askResultsI = countAskResults - 1;
    for (int i = countOps - 1; i >= 0; --i) {
        if (ops[i].title == "cut") {
            disjoinedSets.Join(ops[i].firstI, ops[i].secondI);
        } else if (ops[i].title == "ask") {
            askResults[askResultsI--] = disjoinedSets.AreInOneSet(ops[i].firstI, ops[i].secondI);
        }
    }
    for (int i = 0; i < countAskResults; ++i) {
        std::cout << (askResults[i] ? "YES" : "NO") << std::endl;
    }
    return 0;
}
