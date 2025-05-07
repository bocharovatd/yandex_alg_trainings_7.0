// E. Слабое звено

// В Берляндии по воскресеньям проходит известное шоу — игра «Слабое звено».

// В игре принимают участие n игроков, которые выстраиваются в круг. 
// Каждому игроку сопоставлен рейтинг — некоторое целое число ai.

// Игра проходит в несколько раундов, каждый из которых выглядит следующим образом:

// В очередном раунде принимают участие все ещё не выбывшие игроки.
// Все игроки, которые по рейтингу строго слабее обоих своих соседей по кругу, объявляются слабым звеном и выбывают из игры.
// Все оставшиеся участники сдвигаются чуть плотнее, чтобы снова образовывать круг.
// Игра заканчивается, если после очередного раунда осталось ровно два человека 
// или если после очередного раунда не выбыл ни один человек.
// Иначе начинается новый раунд.
// Можно показать, что если до очередного раунда в игре оставалось хотя бы три участника, 
// то после одного раунда гарантированно останется не менее двух участников.

// Вам нужно выяснить для каждого участника, останется ли он до конца, или номер раунда, в котором он покинет игру.

// Формат ввода
// В первой строке дано одно целое число (2≤n≤200000) — количество участников в игре.

// Вторая строка содержит n целых чисел ai (1≤ai≤200000) — рейтинги всех участников игры в том порядке, 
// в котором они стоят, при этом участник с номером n является соседом участника с номером 1.

// Формат вывода
// Выведите n целых чисел — номер раунда, в котором участник игры с номером i покинет игру, 
// или 0, если этот игрок останется до конца игры.

// Раунды нумеруются последовательными целыми числами, начиная с 1.

#include <iostream>
#include <vector>

class CircularList {
public:
    CircularList() : head(nullptr), size(0) {};
    void Push(int value);
    size_t Size() { return size; }
    void Clear();
    std::vector<int> FindElimRounds();
private:
    struct Node {
        size_t index;
        int value;
        Node* prev;
        Node* next;
    };
    Node* head;
    size_t size;
    void popNode(Node* deleteNode);
    std::vector<Node*> getAllNodes();
};

void CircularList::Push(int value) {
    Node* newNode = new Node;
    newNode->index = size;
    newNode->value = value;
    if (size == 0) {
        newNode->next = newNode;
        newNode->prev = newNode;
        head = newNode;
    } else {
        newNode->next = head;
        newNode->prev = head->prev;
        head->prev->next = newNode;
        head->prev = newNode;
    }
    ++size;
}

void CircularList::popNode(Node* deleteNode) {
    if (size == 1) {
        head = nullptr;
    } else {
        if (head == deleteNode) {
            head = deleteNode->next;
        }
        deleteNode->prev->next = deleteNode->next;
        deleteNode->next->prev = deleteNode->prev;
    }
    delete deleteNode;
    --size;
}

void CircularList::Clear() {
    int value;
    Node* curNode = head;
    while (size != 0) {
        head = curNode->next;
        delete curNode;
        curNode = head;
        --size;
    }
}

std::vector<CircularList::Node*> CircularList::getAllNodes() {
    std::vector<Node*> result(size);
    Node* curNode = head;
    for (int i = 0; i < size; ++i) {
        result[i] = curNode;
        curNode = curNode->next;
    }
    return result;
}

std::vector<int> CircularList::FindElimRounds() {
    std::vector<int> result(size);
    std::vector<Node*> checkNodes = getAllNodes();
    std::vector<Node*> nextCheckNodes = {};
    std::vector<Node*> deleteNodes = {};
    int round = 1;
    while (checkNodes.size() != 0 && size != 2) {
        for (auto node: checkNodes) {
            if (node->prev->value > node->value && node->next->value > node->value) {
                deleteNodes.push_back(node);
                if (nextCheckNodes.size() == 0) {
                    nextCheckNodes.push_back(node->prev);
                    nextCheckNodes.push_back(node->next);
                    continue;
                }
                if (nextCheckNodes[nextCheckNodes.size() - 1] != node->prev) {
                    nextCheckNodes.push_back(node->prev);
                }
                if (nextCheckNodes[0] != node->next) {
                    nextCheckNodes.push_back(node->next);
                }
            }
        }
        for (auto node: deleteNodes) {
            result[node->index] = round;
            popNode(node);
        }
        checkNodes = nextCheckNodes;
        nextCheckNodes = {};
        deleteNodes = {};
        ++round;
    }
    return result;
}

int main() {
    int n;
    std::cin >> n;
    CircularList circularList;
    for (int i = 0; i < n; ++i) {
        int value;
        std::cin >> value;
        circularList.Push(value);
    }
    for (auto round: circularList.FindElimRounds()) {
        std::cout << round << " ";
    }
    circularList.Clear();
    return 0;
}
