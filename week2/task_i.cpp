// I. Максимум на подотрезках с добавлением на отрезке

// Реализуйте эффективную структуру данных 
// для хранения массива и выполнения следующих операций: 
// увеличение всех элементов данного интервала на одно и то же число; 
// поиск максимума на интервале.

// Формат ввода
// В первой строке вводится одно натуральное число 
// N ( 1 ≤ N ≤ 100000) — количество чисел в массиве.

// Во второй строке вводятся N чисел от 0 до 100000 — элементы массива.

// В третьей строке вводится одно натуральное число 
// M (1 ≤ M ≤ 30000) — количество запросов.

// Каждая из следующих M строк представляет собой описание запроса. 
// Сначала вводится одна буква, кодирующая вид запроса 
// (m — найти максимум, a — увеличить все элементы на отрезке).

// Следом за m вводятся два числа — левая и правая граница отрезка.

// Следом за a вводятся три числа — левый и правый концы отрезка и число add, 
// на которое нужно увеличить все элементы данного отрезка массива (0 ≤ add ≤ 100000).

// Формат вывода
// Выведите в одну строку через пробел ответы на каждый запрос m.

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
        void Add(int start, int end, int value);
        long FindMax(int start, int end);
    private:
        const int MIN = -1;
        struct Elem {
            long value;
            long addPromise;
        };
        Elem* segmentTree;
        int segmentTreeSize;
        long add(int start, int end, int value, int elemI, int elemStart, int elemEnd);
        long findMax(int start, int end, int elemI, int elemStart, int elemEnd);
        void fulfillPromise(int elemI);
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
        segmentTree[i].value = MIN;
        segmentTree[i].addPromise = 0;
    }
    for (int i = firstLayerSize - 2; i >= 0; --i) {
        segmentTree[i].value = std::max(segmentTree[i * 2 + 1].value, segmentTree[i * 2 + 2].value);
        segmentTree[i].addPromise = 0;
    }
}

SegmentTree:: ~SegmentTree() {
    delete[] segmentTree;
}

void SegmentTree::fulfillPromise(int elemI) {
    segmentTree[elemI * 2 + 1].value += segmentTree[elemI].addPromise;
    segmentTree[elemI * 2 + 1].addPromise += segmentTree[elemI].addPromise;
    segmentTree[elemI * 2 + 2].value += segmentTree[elemI].addPromise;
    segmentTree[elemI * 2 + 2].addPromise += segmentTree[elemI].addPromise;
    segmentTree[elemI].addPromise = 0;
}

void SegmentTree::Add(int start, int end, int value) {
    add(start - 1, end - 1, value, 0, 0, (segmentTreeSize + 1) / 2 - 1);
}

long SegmentTree::add(int start, int end, int value, int elemI, int elemStart, int elemEnd) {
    if (start <= elemStart && end >= elemEnd) {
        segmentTree[elemI].value += value;
        segmentTree[elemI].addPromise += value;
        return segmentTree[elemI].value;
    } else if (end < elemStart || start > elemEnd) {
        return segmentTree[elemI].value;
    }
    fulfillPromise(elemI);
    segmentTree[elemI].value = std::max(
        add(start, end, value, elemI * 2 + 1, elemStart, elemStart + (elemEnd - elemStart) / 2),
        add(start, end, value, elemI * 2 + 2, elemStart + (elemEnd - elemStart) / 2 + 1, elemEnd)
    );
    return segmentTree[elemI].value;
}

long SegmentTree::FindMax(int start, int end) {
    return findMax(start - 1, end - 1, 0, 0, (segmentTreeSize + 1) / 2 - 1);
}

long SegmentTree::findMax(int start, int end, int elemI, int elemStart, int elemEnd) {
    if (start <= elemStart && end >= elemEnd) {
        return segmentTree[elemI].value;
    } else if (end < elemStart || start > elemEnd) {
        return MIN;
    }
    fulfillPromise(elemI);
    return std::max(
        findMax(start, end, elemI * 2 + 1, elemStart, elemStart + (elemEnd - elemStart) / 2), 
        findMax(start, end, elemI * 2 + 2, elemStart + (elemEnd - elemStart) / 2 + 1, elemEnd)
    );
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
        int start, end;
        switch (type) {
            case 'm':
                std::cin >> start >> end;
                std::cout << segmentTree.FindMax(start, end) << " ";
                break;
            case 'a':
                int value;
                std::cin >> start >> end >> value;
                segmentTree.Add(start, end, value);
                break;
        }
    }
    delete[] arr;
    return 0;
}

