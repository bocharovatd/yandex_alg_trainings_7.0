// E. K-й ноль

// Реализуйте эффективную структуру данных, позволяющую изменять элементы массива и вычислять индекс
// k-го слева нуля на данном отрезке в массиве.

// Формат ввода
// В первой строке вводится одно натуральное число N (1 ≤ N ≤ 200000) — количество чисел в массиве. 
// Во второй строке вводятся N чисел от 0 до 100000 — элементы массива.
// В третьей строке вводится одно натуральное число M (1 ≤ M ≤ 200000 ) — количество запросов.
// Каждая из следующих M строк представляет собой описание запроса. 
// Сначала вводится одна буква, кодирующая вид запроса 
// (s — вычислить индекс k-го нуля, u — обновить значение элемента). 
// Следом за s вводится три числа — левый и правый концы отрезка и число k (1 ≤ k ≤ N). 
// Следом за u вводятся два числа — номер элемента и его новое значение.

// Формат вывода
// Для каждого запроса s выведите результат. Все числа выводите в одну строку через пробел. 
// Если нужного числа нулей на запрашиваемом отрезке нет, выводите − 1 для данного запроса.

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
        void Update(int i, int value);
        int FindKNullI(int start, int end, int k);
        int CountMin(int start, int end);
        int FindKMinI(int k);
    private:
        const int MAX = 100001;
        struct Elem {
            int value;
            int count;
            public:
                bool operator == (const Elem& other) {
                    if (value == other.value && count == other.count) {
                        return true;
                    }
                    return false;
                }
        };
        Elem* segmentTree;
        int segmentTreeSize;
        void update(int elemI);
        int countMin(int start, int end, int elemI, int elemStart, int elemEnd);
        int findKMinI(int k, int elemI, int elemStart, int elemEnd);
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
        segmentTree[i].count = 1;
    }
    for (int i = firstLayerSize - 1 + n; i < segmentTreeSize; ++i) {
        segmentTree[i].value = MAX;
        segmentTree[i].count = 1;
    }
    for (int i = firstLayerSize - 2; i >= 0; --i) {
        Elem leftChild = segmentTree[i * 2 + 1];
        Elem rightChild = segmentTree[i * 2 + 2];
        if (leftChild.value < rightChild.value) {
            segmentTree[i] = leftChild;
        } else if (leftChild.value > rightChild.value) {
            segmentTree[i] = rightChild;
        } else {
            segmentTree[i].value =  leftChild.value;
            segmentTree[i].count = leftChild.count + rightChild.count;
        }
    }
}

SegmentTree::~SegmentTree() {
    delete[] segmentTree;
}

void SegmentTree::Update(int i, int value) {
    int elemI = (segmentTreeSize + 1) / 2 - 1 + (i - 1);
    segmentTree[elemI].value = value;
    if (elemI != 0) {
        update((elemI - 1) / 2);
    }
}

void SegmentTree::update(int elemI) {
    Elem leftChild = segmentTree[elemI * 2 + 1];
    Elem rightChild = segmentTree[elemI * 2 + 2];
    Elem newElem;
    if (leftChild.value < rightChild.value) {
        newElem = leftChild;
    } else if (leftChild.value > rightChild.value) {
        newElem = rightChild;
    } else {
        newElem.value =  leftChild.value;
        newElem.count = leftChild.count + rightChild.count;
    }
    if (!(newElem == segmentTree[elemI])) {
        segmentTree[elemI] = newElem;
        if (elemI != 0) {
            update((elemI - 1) / 2);
        }
    }
}

int SegmentTree::FindKNullI(int start, int end, int k) {
    if (segmentTree[0].value != 0) {
        return -1;
    }
    int countNullsBeforeStart = CountMin(1, start - 1);
    int kMinI = FindKMinI(countNullsBeforeStart + k);
    if (kMinI < start || kMinI > end) {
        return -1;
    }
    return kMinI;
}

int SegmentTree::CountMin(int start, int end) {
    return countMin(start - 1, end - 1, 0, 0, (segmentTreeSize + 1) / 2 - 1);
}

int SegmentTree::countMin(int start, int end, int elemI, int elemStart, int elemEnd) {
    if (start <= elemStart && end >= elemEnd) {
        return segmentTree[elemI].count;
    } else if (end < elemStart || start > elemEnd) {
        return 0;
    }
    int count = 0;
    if (segmentTree[elemI * 2 + 1].value == segmentTree[elemI].value) {
        count += countMin(start, end, elemI * 2 + 1, elemStart, elemStart + (elemEnd - elemStart) / 2);
    }
    if (segmentTree[elemI * 2 + 2].value == segmentTree[elemI].value) {
        count += countMin(start, end, elemI * 2 + 2, elemStart + (elemEnd - elemStart) / 2 + 1, elemEnd);
    }
    return count;
}

int SegmentTree::FindKMinI(int k) {
    if (k > segmentTree[0].count) {
        return -1;
    }
    return findKMinI(k - 1, 0, 0, (segmentTreeSize + 1) / 2 - 1) + 1;
}

int SegmentTree::findKMinI(int k, int elemI, int elemStart, int elemEnd) {
    if (elemStart == elemEnd) {
        return elemStart;
    }
    int countMinLeft = 0;
    if (segmentTree[elemI * 2 + 1].value == segmentTree[elemI].value) {
        countMinLeft = segmentTree[elemI * 2 + 1].count;
    }
    if (k < countMinLeft) {
        return findKMinI(k, elemI * 2 + 1, elemStart, elemStart + (elemEnd - elemStart) / 2);
    }
    return findKMinI(k - countMinLeft, elemI * 2 + 2, elemStart + (elemEnd - elemStart) / 2 + 1, elemEnd);
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
            case 's':
                int start;
                int end;
                int k;
                std::cin >> start >> end >> k;
                std::cout << segmentTree.FindKNullI(start, end, k) << " ";
                break;
            case 'u':
                int index;
                int newValue;
                std::cin >> index >> newValue;
                segmentTree.Update(index, newValue);
                break;
        }
    }
    delete[] arr;
    return 0;
}