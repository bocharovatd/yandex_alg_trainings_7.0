// H. Дерево отрезков с операцией на отрезке

// Реализуйте эффективную структуру данных для хранения элементов и увеличения 
// нескольких подряд идущих элементов на одно и то же число.

// Формат ввода
// В первой строке вводится одно натуральное число N (1 ⩽ N ⩽ 100000) — количество чисел в массиве.

// Во второй строке вводятся N чисел от 0 до 100000 — элементы массива.

// В третьей строке вводится одно натуральное число M (1⩽M⩽30000) — количество запросов.

// Каждая из следующих M строк представляет собой описание запроса. Сначала вводится одна буква, кодирующая вид запроса (g — получить текущее значение элемента по его номеру, а — увеличить все элементы на отрезке).

// Следом за g вводится одно число — номер элемента.

// Следом за a вводится три числа — левый и правый концы отрезка и число add, 
// на которое нужно увеличить все элементы данного отрезка массива (0 ⩽ add ⩽ 100000).

// Формат вывода
// Выведите в одну строку через пробел ответы на каждый запрос g.

#include <iostream>
#include <math.h>

bool isPowOfTwo(int x) {
    return x && !(x & (x - 1));
}

struct Request {
    char type;
    int firstParam;
    int secondParam;
};

class SegmentTree {
    public:
        SegmentTree(int* arr, int n);
        ~SegmentTree();
        long GetElemByIndex(int index);
        void Add(int start, int end, int value);
    private:
        struct Elem {
            long value;
            long addPromise;
        };
        Elem* segmentTree;
        int segmentTreeSize;
        void add(int start, int end, int value, int elemI, int elemStart, int elemEnd);
        long getElemByIndex(int index, int elemI, int elemStart, int elemEnd);
};

SegmentTree::SegmentTree(int* arr, int n) {
    int countNeutralElems;
    for (countNeutralElems = 0; 
        !isPowOfTwo(n + countNeutralElems);
        ++countNeutralElems) {}
    int firstLayerSize = n + countNeutralElems;
    segmentTreeSize = firstLayerSize * 2 - 1;
    segmentTree = new Elem[segmentTreeSize];
    for (int i = firstLayerSize - 1; i < firstLayerSize - 1 + n; ++i) {
        segmentTree[i].value = arr[i - (firstLayerSize - 1)];
        segmentTree[i].addPromise = 0;
    }
    for (int i = firstLayerSize - 1 + n; i < segmentTreeSize; ++i) {
        segmentTree[i].addPromise = 0;
    }
    for (int i = firstLayerSize - 2; i >= 0; --i) {
        segmentTree[i].addPromise = 0;
    }
}

SegmentTree:: ~SegmentTree() {
    delete[] segmentTree;
}

void SegmentTree::Add(int start, int end, int value) {
    add(start - 1, end - 1, value, 0, 0, (segmentTreeSize + 1) / 2 - 1);
}

void SegmentTree::add(int start, int end, int value, int elemI, int elemStart, int elemEnd) {
    if (start <= elemStart && end >= elemEnd) {
        segmentTree[elemI].addPromise += value;
        return;
    } else if (end < elemStart || start > elemEnd) {
        return;
    }

    add(start, end, value, elemI * 2 + 1, elemStart, elemStart + (elemEnd - elemStart) / 2);
    add(start, end, value, elemI * 2 + 2, elemStart + (elemEnd - elemStart) / 2 + 1, elemEnd);
}

long SegmentTree::GetElemByIndex(int index) {
    return getElemByIndex(index - 1, 0, 0, (segmentTreeSize + 1) / 2 - 1);
}

long SegmentTree::getElemByIndex(int index, int elemI, int elemStart, int elemEnd) {
    if (elemStart == elemEnd && elemStart == index) {
        segmentTree[elemI].value += segmentTree[elemI].addPromise;
        segmentTree[elemI].addPromise = 0;
        return segmentTree[elemI].value;
    } else if (index < elemStart || index > elemEnd) {
        return -1;
    }

    segmentTree[elemI * 2 + 1].addPromise += segmentTree[elemI].addPromise;
    segmentTree[elemI * 2 + 2].addPromise += segmentTree[elemI].addPromise;
    segmentTree[elemI].addPromise = 0;
    long leftChildResult = getElemByIndex(index, elemI * 2 + 1, elemStart, elemStart + (elemEnd - elemStart) / 2);
    if (leftChildResult != -1) {
        return leftChildResult;
    }
    long rightChildResult = getElemByIndex(index,  elemI * 2 + 2, elemStart + (elemEnd - elemStart) / 2 + 1, elemEnd);
    return rightChildResult;
}

int main() {
    int n;
    std::cin >> n;
    int* arr = new int[n];
    for (int i = 0; i < n; ++i) {
        std::cin >> arr[i];
    }
    int m;
    std::cin >> m;
    SegmentTree segmentTree(arr, n);
    for (int i = 0; i < m; ++i) {
        char type;
        std::cin >> type;
        switch (type) {
            case 'g':
                int index;
                std::cin >> index;
                std::cout << segmentTree.GetElemByIndex(index) << std::endl;
                break;
            case 'a':
                int start;
                int end;
                int value;
                std::cin >> start >> end >> value;
                segmentTree.Add(start, end, value);
                break;
        }
    }
    delete[] arr;
    return 0;
}