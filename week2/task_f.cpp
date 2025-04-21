// F. Ближайшее большее число справа

// Дан массив a из n чисел. Нужно обрабатывать запросы:

// 0. set(i, x) — присвоить новое значение элементу массива a[i] = x;
// 1. get(i, x) — найти min k:k ≥ i и ak ≥ x.

// Формат ввода
// Первая строка входных данных содержит два числа: длину массива n и количество запросов m (1 ≤n,m≤200000).

// Во второй строке записаны n целых чисел – элементы массива a (0 ≤ ai ≤2 00000).

// Следующие m строк содержат запросы, каждый запрос содержит три числа t, i, x. 
// Первое число t равно 0 или 1 – тип запроса. 
// t=0 означает запрос типа set, t=1 соответствует запросу типа get, 1 ≤ i ≤ n, 0 ≤ x ≤ 200000. 
// Элементы массива нумеруются с единицы.

// Формат вывода
// На каждый запрос типа get на отдельной строке выведите соответствующее значение k. 
// Если такого k не существует, выведите −1.

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
        int GetFirstGeXI(int index, int x);
    private:
        const int MIN = -1;
        struct Elem {
            int value;
        };
        Elem* segmentTree;
        int segmentTreeSize;
        int getFirstGeXI(int index, int x, int elemI, int elemStart, int elemEnd);
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

int SegmentTree::GetFirstGeXI(int index, int x) {
    return getFirstGeXI(index - 1, x, 0, 0, (segmentTreeSize + 1) / 2 - 1) + 1;
}

int SegmentTree::getFirstGeXI(int index, int x, int elemI, int elemStart, int elemEnd) {
    if (elemEnd < index || segmentTree[elemI].value < x) {
        return -2;
    }

    if (elemStart == elemEnd) {
        return elemStart;
    }

    int leftResult = getFirstGeXI(index, x, elemI * 2 + 1, elemStart, elemStart + (elemEnd - elemStart) / 2);
    if (leftResult != -2) {
        return leftResult;
    }

    int rightResult = getFirstGeXI(index, x, elemI * 2 + 2, elemStart + (elemEnd - elemStart) / 2 + 1, elemEnd);
    return rightResult;
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
    int m;
    std::cin >> n >> m;
    int* arr = new int[n];
    for (int i = 0; i < n; ++i) {
        std::cin >> arr[i];
    }
    SegmentTree segmentTree(arr, n);
    for (int i = 0; i < m; ++i) {
        int type;
        int index;
        std::cin >> type;
        switch (type) {
            case 0:
                int newValue;
                std::cin >> index >> newValue;
                segmentTree.Update(index, newValue);
                break;
            case 1:
                int x;
                std::cin >> index >> x;
                std::cout << segmentTree.GetFirstGeXI(index, x) << std::endl;
                break;
        }
    }

    delete[] arr;
    return 0;
}