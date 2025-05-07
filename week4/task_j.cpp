// J. Простая река

// Во Флатландии протекает богатая рыбой река Большой Флат. 
// Много лет назад река была поделена между n рыболовными предприятиями, каждое из которых получило непрерывный отрезок реки. 
// При этом i-е предприятие, если рассматривать их по порядку, начиная от истока, изначально получило отрезок реки длиной a i.

// С тех пор с рыболовными предприятиями во Флатландии k раз происходили различные события. 
// Каждое из событий было одного из двух типов: банкротство некоторого предприятия или разделение некоторого предприятия на два.

// При некоторых событиях отрезок реки, принадлежащий предприятию, с которым это событие происходит, делится на две части. 
// Каждый такой отрезок имеет длину большую или равную 2. Деление происходит по следующему правилу: 
// если отрезок имеет чётную длину, то он делится на две равные части; иначе он делится на две части, 
// длины которых различаются ровно на единицу, при этом часть, которая ближе к истоку реки, имеет меньшую длину.

// При банкротстве предприятия происходит следующее: отрезок реки, принадлежавший обанкротившемуся предприятию, 
// переходит к его соседям. Если у обанкротившегося предприятия один сосед, 
// то этому соседу целиком передается отрезок реки обанкротившегося предприятия. 
// Если же соседей двое, то отрезок реки делится на две части описанным выше способом, 
// после чего каждый из соседей присоединяет к своему отрезку ближайшую к нему часть.

// При разделении предприятия отрезок реки, принадлежавший разделяемому предприятию, 
// всегда делится на две части описанным выше способом. Разделившееся предприятие ликвидируется, 
// и образуются два новых предприятия. Таким образом, после каждого события каждое предприятие владеет некоторым отрезком реки.

// Министерство финансов Флатландии предлагает ввести налог на рыболовные предприятия, 
// пропорциональный квадрату длины отрезка реки, принадлежащего соответствующему предприятию. 
// Чтобы проанализировать, как будет работать этот налог, министр хочет по имеющимся данным узнать, 
// как изменялась величина, равная сумме квадратов длин отрезков реки, принадлежащих предприятиям, 
// после каждого произошедшего события.

// Требуется написать программу, которая по заданному начальному разделению реки между предприятиями и списку событий, 
// происходивших с предприятиями, определит, чему равна сумма квадратов длин отрезков реки, принадлежащих предприятиям, 
// в начальный момент времени и после каждого события.

// Формат ввода
// Первая строка ввода содержит число n — исходное количество предприятий (2 ≤ n ≤ 100000).
// Вторая строка входного файла содержит n целых чисел a1, a2, ..., an — длины исходных отрезков реки.
// Третья строка входного файла содержит целое число k — количество событий, происходивших с предприятиями (1 ≤ k ≤ 100000).

#include <iostream>
#include <list>

class List {
public:
    List() : curNode(nullptr), curI(0), sumOfSquares(0) {};
    ~List();
    void PushBack(unsigned long value);
    void Split(size_t index);
    void SplitBetweenNeighbours(size_t index);
    unsigned long SumOfSquares() { return sumOfSquares; }
private:
    struct Node {
        unsigned long value;
        Node* prev;
        Node* next;
    };
    Node* curNode;
    void moveTo(size_t index);
    void split();
    void splitBetweenNeighbours();
    size_t curI;
    unsigned long sumOfSquares;
};

List::~List() {
    if (curNode != nullptr) {
        Node* curLeftNode = curNode->prev;
        Node* curRightNode = curNode->next;
        while (curLeftNode != nullptr) {
            Node* deleteNode = curLeftNode;
            curLeftNode = curLeftNode->prev;
            delete deleteNode;
        }
        while (curRightNode != nullptr) {
            Node* deleteNode = curRightNode;
            curRightNode = curRightNode->next;
            delete deleteNode;
        }
        delete curNode;
    }
}

void List::PushBack(unsigned long value) {
    Node* newNode = new Node;
    newNode->value = value;
    newNode->prev = curNode;
    newNode->next = nullptr;
    if (curNode != nullptr) {
        curNode->next = newNode;
    }
    curNode = newNode;
    ++curI;
    sumOfSquares += value * value;
}

void List::Split(size_t index) {
    moveTo(index);
    split();
}

void List::SplitBetweenNeighbours(size_t index) {
    moveTo(index);
    splitBetweenNeighbours();
}

void List::moveTo(size_t index) {
    if (curI > index) {
        while (curI != index) {
            curNode = curNode->prev;
            --curI;
        }
    } else if (curI < index) {
        while (curI != index) {
            curNode = curNode->next;
            ++curI;
        }
    }
}

void List::split() {
    Node* leftNode = new Node;
    leftNode->value = curNode->value / 2;
    leftNode->prev = curNode->prev;
    if (curNode->prev) {
        curNode->prev->next = leftNode;
    }

    Node* rightNode = new Node;
    rightNode->value = curNode->value / 2 + curNode->value % 2;
    rightNode->next = curNode->next;
    if (curNode->next) {
        curNode->next->prev = rightNode;
    }

    leftNode->next = rightNode;
    rightNode->prev = leftNode;

    sumOfSquares -=  curNode->value * curNode->value;
    sumOfSquares += leftNode->value * leftNode->value;
    sumOfSquares += rightNode->value * rightNode->value;

    delete curNode;
    curNode = leftNode;
}

void List::splitBetweenNeighbours() {
    Node* newCurNode;
    if (curNode->prev == nullptr) {
        sumOfSquares -= curNode->next->value * curNode->next->value;
        curNode->next->value += curNode->value;
        sumOfSquares += curNode->next->value * curNode->next->value;
        curNode->next->prev = nullptr;

        newCurNode = curNode->next;
    } else if (curNode->next == nullptr) {
        sumOfSquares -= curNode->prev->value * curNode->prev->value;
        curNode->prev->value += curNode->value;
        sumOfSquares += curNode->prev->value * curNode->prev->value;
        curNode->prev->next = nullptr;

        newCurNode = curNode->prev;
        --curI;
    } else {
        sumOfSquares -= curNode->prev->value * curNode->prev->value;
        curNode->prev->value += curNode->value / 2;
        sumOfSquares += curNode->prev->value * curNode->prev->value;
        curNode->prev->next = curNode->next;

        sumOfSquares -= curNode->next->value * curNode->next->value;
        curNode->next->value += curNode->value / 2 + curNode->value % 2;
        sumOfSquares += curNode->next->value * curNode->next->value;
        curNode->next->prev = curNode->prev;

        newCurNode = curNode->next;
    }
    
    sumOfSquares -= curNode->value * curNode->value;

    delete curNode;
    curNode = newCurNode;
}

int main() {
    int n;
    std::cin >> n;
    List list;
    for (int i = 0; i < n; ++i) {
        unsigned long value;
        std::cin >> value;
        list.PushBack(value);
    }
    int k;
    std::cin >> k;
    std::cout << list.SumOfSquares() << std::endl;
    for (int i = 0; i < k; ++i) {
        char command;
        size_t index;
        std::cin >> command >> index;
        switch (command) {
            case '1':
                list.SplitBetweenNeighbours(index);
                std::cout << list.SumOfSquares() << std::endl;
                break;
            case '2':
                list.Split(index);
                std::cout << list.SumOfSquares() << std::endl;
                break;
        }
    }
    return 0;
}