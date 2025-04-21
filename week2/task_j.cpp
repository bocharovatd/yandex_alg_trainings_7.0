// J. Присваивание на отрезке, сравнение подотрезков*

// Реализуйте структуру данных, которая позволяет выполнять две операции:

// Присвоить всем элементам на отрезке от L до R значение K
// Поэлементно сравнить все числа на отрезках длины K, начинающихся с позиций L и R
// Формат ввода
// В первой строке записано число 
// N — количество элементов в последовательности (1 ≤ N ≤ 100000). 
// Во второй строке записано N целых чисел — начальные значения последовательности, 
// все числа имеют значения от 1 до 100000.

// В третьей строке записано число 
// Q — количество операций сравнения и присваивания (1 ≤ Q ≤ 100000). 
// Следующие Q строк содержат описания операций: четыре числа T, L, R и K.

// Если T=0, то необходимо всем числам с индексами с L до R включительно присвоить значение K (1 ≤ L ≤ R ≤ N, 1 ≤ K ≤ 100000).

// Если T=1, то необходимо сравнить подотрезки, начинающиеся с позиций L и R и имеющие длину K(1 ≤ L,R ≤ N − K + 1 ≤ L, K > 0).

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
        void Change(int start, int end, int value);
        long long FindHash(int start, int end);
    private:
        const int NEUTRAL = 0;
        const int BASE = 100003;
        const int MOD = 1000000007;
        long long* powP;
        long long* hashPowP;
        struct Elem {
            int start;
            int end;
            long long hash;
            long changePromise;
        };
        Elem* segmentTree;
        int segmentTreeSize;
        Elem change(int start, int end, int value, int elemI, int elemStart, int elemEnd);
        Elem findHash(int start, int end, int elemI, int elemStart, int elemEnd);
        void fulfillPromise(int elemI);
        long long mergeHash(Elem leftChild, Elem rightChild);
};

long long SegmentTree::mergeHash(Elem leftChild, Elem rightChild) {
    return (leftChild.hash + powP[rightChild.start - leftChild.start] * rightChild.hash) % MOD;
}

SegmentTree::SegmentTree(int* arr, int n) {
    powP = new long long[100001];
    hashPowP = new long long[100001];
    powP[0] = 1;
    hashPowP[0] = 1;
    for (int i = 1; i < 100001; ++i) {
        powP[i] = powP[i - 1] * BASE % MOD;
        hashPowP[i] = (hashPowP[i - 1] + powP[i]) % MOD;
    }

    int countNeutralElems;
    for (countNeutralElems = 0; 
        !isPowOfTwo(n + countNeutralElems);
        ++countNeutralElems) {}
    int firstLayerSize = n + countNeutralElems;
    segmentTreeSize = firstLayerSize * 2 - 1;
    segmentTree = new Elem[segmentTreeSize];
    for (int i = firstLayerSize - 1; i < firstLayerSize - 1 + n; ++i) {
        segmentTree[i].start = i - (firstLayerSize - 1);
        segmentTree[i].end = i - (firstLayerSize - 1);
        segmentTree[i].hash = arr[i - (firstLayerSize - 1)];
        segmentTree[i].changePromise = 0;
    }
    for (int i = firstLayerSize - 1 + n; i < segmentTreeSize; ++i) {
        segmentTree[i].start = i - (firstLayerSize - 1);
        segmentTree[i].end = i - (firstLayerSize - 1);
        segmentTree[i].hash = NEUTRAL;
        segmentTree[i].changePromise = 0;
    }
    for (int i = firstLayerSize - 2; i >= 0; --i) {
        segmentTree[i].start = segmentTree[i * 2 + 1].start;
        segmentTree[i].end = segmentTree[i * 2 + 2].end;
        segmentTree[i].hash = mergeHash(segmentTree[i * 2 + 1], segmentTree[i * 2 + 2]);
        segmentTree[i].changePromise = 0;
    }
}

SegmentTree:: ~SegmentTree() {
    delete[] segmentTree;
    delete[] powP;
}

void SegmentTree::fulfillPromise(int elemI) {
    if (segmentTree[elemI].changePromise != 0) {
        segmentTree[elemI * 2 + 1].hash = (segmentTree[elemI].changePromise * hashPowP[segmentTree[elemI * 2 + 1].end - segmentTree[elemI * 2 + 1].start]) % MOD;
        segmentTree[elemI * 2 + 1].changePromise = segmentTree[elemI].changePromise;
        segmentTree[elemI * 2 + 2].hash = (segmentTree[elemI].changePromise * hashPowP[segmentTree[elemI * 2 + 2].end - segmentTree[elemI * 2 + 2].start]) % MOD;
        segmentTree[elemI * 2 + 2].changePromise = segmentTree[elemI].changePromise;
        segmentTree[elemI].changePromise = 0;
    }
}

void SegmentTree::Change(int start, int end, int value) {
    change(start - 1, end - 1, value, 0, 0, (segmentTreeSize + 1) / 2 - 1).hash;
}

SegmentTree::Elem SegmentTree::change(int start, int end, int value, int elemI, int elemStart, int elemEnd) {
    if (start <= elemStart && end >= elemEnd) {
        segmentTree[elemI].hash = value * hashPowP[elemEnd - elemStart] % MOD;
        segmentTree[elemI].changePromise = value % MOD;
        return segmentTree[elemI];
    } else if (end < elemStart || start > elemEnd) {
        return segmentTree[elemI];
    }
    fulfillPromise(elemI);
    segmentTree[elemI].hash = mergeHash(
        change(start, end, value, elemI * 2 + 1, elemStart, elemStart + (elemEnd - elemStart) / 2),
        change(start, end, value, elemI * 2 + 2, elemStart + (elemEnd - elemStart) / 2 + 1, elemEnd)
    );
    return segmentTree[elemI];
}

long long SegmentTree::FindHash(int start, int end) {
    return findHash(start - 1, end - 1, 0, 0, (segmentTreeSize + 1) / 2 - 1).hash;
}

SegmentTree::Elem SegmentTree::findHash(int start, int end, int elemI, int elemStart, int elemEnd) {
    if (start <= elemStart && end >= elemEnd) {
        return segmentTree[elemI];
    } else if (end < elemStart || start > elemEnd) {
        Elem neutralElem = segmentTree[elemI];
        neutralElem.hash = NEUTRAL;
        return neutralElem;
    }
    fulfillPromise(elemI);
    Elem newElem = segmentTree[elemI];
    Elem leftResult = findHash(start, end, elemI * 2 + 1, elemStart, elemStart + (elemEnd - elemStart) / 2);
    Elem rightResult = findHash(start, end, elemI * 2 + 2, elemStart + (elemEnd - elemStart) / 2 + 1, elemEnd);
    if (leftResult.hash == NEUTRAL) {
        newElem.start = rightResult.start;
        newElem.hash = rightResult.hash;
    } else if (rightResult.hash == NEUTRAL) {
        newElem.end = leftResult.end;
        newElem.hash = leftResult.hash;
    } else {
        newElem.start = leftResult.start;
        newElem.end = rightResult.end;
        newElem.hash = mergeHash(leftResult, rightResult);
    }
    return newElem; 
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
            case '0':
                int start, end, value;
                std::cin >> start >> end >> value;
                segmentTree.Change(start, end, value);
                break;
            case '1':
                int startFirst, startSecond, length;
                std::cin >> startFirst >> startSecond >> length;
                bool areEqual = segmentTree.FindHash(startFirst, startFirst + length - 1) == segmentTree.FindHash(startSecond, startSecond + length - 1);
                std::cout << (areEqual ? "+" : "-");
                break;
        }
    }
    delete[] arr;
    return 0;
}
