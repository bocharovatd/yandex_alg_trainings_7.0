// D. Максимум на подотрезках с изменением элемента

// Реализуйте эффективную структуру данных, 
// позволяющую изменять элементы массива и вычислять максимальное значение 
// из нескольких подряд идущих элементов.

// Формат ввода
// В первой строке вводится одно натуральное число N (1 ≤ N ≤ 100000)  — количество чисел в массиве.

// Во второй строке вводятся 
// N чисел от 0 до 100000 — элементы массива.

// В третьей строке вводится одно натуральное число M (1 ≤ M ≤ 30000) — количество запросов.

// Каждая из следующих 
// M строк представляет собой описание запроса. Сначала вводится одна буква, кодирующая вид запроса 
// (s — вычислить максимум, u — обновить значение элемента).

// Следом за s вводятся два числа — номера левой и правой границы отрезка.

// Следом за u вводятся два числа — номер элемента и его новое значение.

// Формат вывода
// Для каждого запроса s выведите результат. Все числа выводите в одну строку через пробел.

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
        int FindMax(int start, int end);
        void Update(int i, int value);
    private:
        const int MIN = -1;
        struct Elem {
            int value;
        };
        Elem* segmentTree;
        int segmentTreeSize;
        int findMax(int start, int end, int elemI, int elemStart, int elemEnd);
        void update(int elemI);
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
    }
    for (int i = firstLayerSize - 1 + n; i < segmentTreeSize; ++i) {
        segmentTree[i].value = MIN;
    }
    for (int i = firstLayerSize - 2; i >= 0; --i) {
        segmentTree[i].value = std::max(segmentTree[i * 2 + 1].value, segmentTree[i * 2 + 2].value);
    }
}

SegmentTree:: ~SegmentTree() {
    delete[] segmentTree;
}

int SegmentTree::FindMax(int start, int end) {
    return findMax(start - 1, end - 1, 0, 0, (segmentTreeSize + 1) / 2 - 1);
}

int SegmentTree::findMax(int start, int end, int elemI, int elemStart, int elemEnd) {
    if (start <= elemStart && end >= elemEnd) {
        return segmentTree[elemI].value;
    } else if (end < elemStart || start > elemEnd) {
        return MIN;
    }
    return std::max(
            findMax(start, end, elemI * 2 + 1, elemStart, elemStart + (elemEnd - elemStart) / 2), 
            findMax(start, end, elemI * 2 + 2, elemStart + (elemEnd - elemStart) / 2 + 1, elemEnd)
        );
}

void SegmentTree::Update(int i, int value) {
    int elemI = (segmentTreeSize + 1) / 2 - 1 + (i - 1);
    segmentTree[elemI].value = value;
    if (elemI != 0) {
        update((elemI - 1) / 2);
    }
}

void SegmentTree::update(int elemI) {
    int newMax = std::max(segmentTree[elemI * 2 + 1].value, segmentTree[elemI * 2 + 2].value);
    if (newMax != segmentTree[elemI].value) {
        segmentTree[elemI].value = newMax;
        if (elemI != 0) {
            update((elemI - 1) / 2);
        }
    }
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
    Request* requests = new Request[m];
    for (int i = 0; i < m; ++i) {
        char type;
        int firstParam;
        int secondParam;
        std::cin >> type >> firstParam >> secondParam;
        requests[i] = {.type = type, .firstParam = firstParam, .secondParam = secondParam};
    }
    SegmentTree segmentTree(arr, n);
    for (int i = 0; i < m; ++i) {
        switch (requests[i].type) {
            case 's':
                std::cout << segmentTree.FindMax(requests[i].firstParam, requests[i].secondParam) << " ";
                break;
            case 'u':
                segmentTree.Update(requests[i].firstParam, requests[i].secondParam);
                break;
        }
    }
    delete[] arr;
    delete[] requests;
    return 0;
}