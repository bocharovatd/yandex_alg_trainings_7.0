// G. Нолики

// Дедус любит давать своим ученикам сложные задачки. На этот раз он придумал такую задачу:

// Рейтинг всех его учеников записан в массив A. Запросы Дедуса таковы:
// 1. Изменить рейтинг i-го ученика на число x
// 2. Найти максимальную последовательность подряд идущих ноликов в массиве A на отрезке [l,r].

// Формат ввода
// В первой строке входного файла записано число N (1 ≤ N ≤ 500000) — количество учеников. 
// Во второй строке записано N чисел — их рейтинги, числа по модулю не превосходящие 1000
// (по количеству задач, которые ученик решил или не решил за время обучения). 
// В третьей строке записано число M (1 ≤ M ≤ 50000) — количество запросов. 
// Каждая из следующих M строк содержит описания запросов:
// <<UPDATE i x>> — обновить i-ый элемент массива значением x (1 ≤ i ≤ N, ∣x∣ ≤ 1000)
// <<QUERY l r>> — найти длину максимальной последовательности из нулей на отрезке с l по r. (1 ≤ l ≤ r ≤ N)

// Формат вывода
// В выходной файл выведите ответы на запросы <<QUERY>> в том же порядке, что и во входном файле

#include <iostream>
#include <math.h>
#include <string>
#include <algorithm>

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
        int FindLargestNullSeg(int start, int end);
    private:
        struct Elem {
            int nullSeg;
            int nullPref;
            int nullSuf;

            public:
            bool operator == (const Elem& other) {
                if (nullSeg == other.nullSeg && nullPref == other.nullPref && nullSuf == other.nullSuf) {
                    return true;
                }
                return false;
            }
        };
        Elem* segmentTree;
        int segmentTreeSize;
        int segmentLength(int i);
        Elem updateElem(Elem leftChild, int leftChildI, Elem rightChild, int rightChildI);
        void update(int elemI);
        Elem findLargestNullSeg(int start, int end, int elemI, int elemStart, int elemEnd);
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
        if (arr[i - (firstLayerSize - 1)] == 0) {
            segmentTree[i] = {.nullSeg = 1, .nullPref = 1, .nullSuf = 1};
        } else {
            segmentTree[i] = {.nullSeg = 0, .nullPref = 0, .nullSuf = 0};
        }
    }
    for (int i = firstLayerSize - 1 + n; i < segmentTreeSize; ++i) {
        segmentTree[i] = {.nullSeg = 0, .nullPref = 0, .nullSuf = 0};
    }
    for (int i = firstLayerSize - 2; i >= 0; --i) {
        Elem leftChild = segmentTree[i * 2 + 1];
        Elem rightChild = segmentTree[i * 2 + 2];
        segmentTree[i] = updateElem(leftChild, segmentLength(i * 2 + 1), rightChild, segmentLength(i * 2 + 2));
    }
}

SegmentTree::~SegmentTree() {
    delete[] segmentTree;
}

int SegmentTree::segmentLength(int i) {
    int largestSegSize = (segmentTreeSize + 1) / 2;
    int curLevelFromTop = log2(i + 1);
    return largestSegSize / pow(2, curLevelFromTop);
}

SegmentTree::Elem SegmentTree::updateElem(Elem leftChild, int leftChildLength, Elem rightChild, int rightChildLength) {
    Elem newElem;
    newElem.nullSeg = std::max({leftChild.nullSeg, rightChild.nullSeg, leftChild.nullSuf + rightChild.nullPref});    
    if (leftChildLength == leftChild.nullSeg) {
        newElem.nullPref = leftChild.nullSeg + rightChild.nullPref;
    } else {
        newElem.nullPref = leftChild.nullPref;
    }
    if (rightChildLength == rightChild.nullSeg) {
        newElem.nullSuf = leftChild.nullSuf + rightChild.nullSeg;
    } else {
        newElem.nullSuf = rightChild.nullSuf;
    }
    return newElem;
}

void SegmentTree::Update(int i, int value) {
    int elemI = (segmentTreeSize + 1) / 2 - 1 + (i - 1);
    if (value == 0) {
        segmentTree[elemI] = {.nullSeg = 1, .nullPref = 1, .nullSuf = 1};
    } else {
        segmentTree[elemI] = {.nullSeg = 0, .nullPref = 0, .nullSuf = 0};
    }
    if (elemI != 0) {
        update((elemI - 1) / 2);
    }
}

void SegmentTree::update(int elemI) {
    Elem leftChild = segmentTree[elemI * 2 + 1];
    Elem rightChild = segmentTree[elemI * 2 + 2];
    Elem newElem = updateElem(leftChild, segmentLength(elemI * 2 + 1), rightChild, segmentLength(elemI * 2 + 2));

    if (!(newElem == segmentTree[elemI])) {
        segmentTree[elemI] = newElem;
        if (elemI != 0) {
            update((elemI - 1) / 2);
        }
    }
}

int SegmentTree::FindLargestNullSeg(int start, int end) {
    return findLargestNullSeg(start - 1, end - 1, 0, 0, (segmentTreeSize + 1) / 2 - 1).nullSeg;
}

SegmentTree::Elem SegmentTree::findLargestNullSeg(int start, int end, int elemI, int elemStart, int elemEnd) {
    if (start <= elemStart && end >= elemEnd) {
        return segmentTree[elemI];
    } else if (end < elemStart || start > elemEnd) {
        return Elem{0, 0, 0};
    }

    Elem leftChildResult = findLargestNullSeg(start, end, elemI * 2 + 1, elemStart, elemStart + (elemEnd - elemStart) / 2);
    Elem rightChildResult = findLargestNullSeg(start, end, elemI * 2 + 2, elemStart + (elemEnd - elemStart) / 2 + 1, elemEnd);

    return updateElem(leftChildResult, segmentLength(elemI * 2 + 1), rightChildResult, segmentLength(elemI * 2 + 2));
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
        std::string type;
        std::cin >> type;
        switch (type[0]) {
            case 'Q':
                int start;
                int end;
                std::cin >> start >> end;
                std::cout << segmentTree.FindLargestNullSeg(start, end) << std::endl;
                break;
            case 'U':
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