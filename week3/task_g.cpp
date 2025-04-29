// G. Сумма на отрезке

// Дан массив из N элементов, нужно научиться находить сумму чисел на отрезке.

// Формат ввода
// Первая строка входного файла содержит два целых числа N и K 
// — количество чисел в массиве и количество запросов (1⩽N⩽100000, 0⩽K⩽100000). 
// Следующие K строк содержат следующие запросы:

// A i x — присвоить i-му элементу массива значение x (1⩽i⩽n, 0⩽x⩽10^9);
// Q l r — найти сумму чисел в массиве на позициях от l до r (1⩽l⩽r⩽n).
// Изначально в массиве живут нули.

// Формат вывода
// На каждый запрос вида Q l r нужно вывести единственное число — сумму на отрезке.

#include <iostream>

class BinaryIndexedTree {
    public:
    BinaryIndexedTree(int n);
    ~BinaryIndexedTree();
    void ChangeI(int i, int value);
    unsigned long long CalcSum(int start, int end);
    private:
    unsigned long long* binaryIndexTree;
    int binaryIndexTreeSize;
    unsigned long long calcPrefixSum(int i);
};

BinaryIndexedTree::BinaryIndexedTree(int n) {
    binaryIndexTree = new unsigned long long[n](); 
    binaryIndexTreeSize = n;
}

BinaryIndexedTree::~BinaryIndexedTree() {
    delete[] binaryIndexTree;
}

unsigned long long BinaryIndexedTree::CalcSum(int start, int end) {
    if (start == 0) {
        return calcPrefixSum(end);
    }
    return calcPrefixSum(end) - calcPrefixSum(start - 1);
}

unsigned long long BinaryIndexedTree::calcPrefixSum(int i) {
    unsigned long long sum = 0;
    do {
        sum += binaryIndexTree[i];
        i = (i & (i + 1)) - 1;
    } while (i > 0);
    return sum;
}

void BinaryIndexedTree::ChangeI(int i, int value) {
    int curValue = CalcSum(i, i);
    while(i < binaryIndexTreeSize) {
        binaryIndexTree[i] += -curValue + value;
        i = i | (i + 1);
    }
}

int main() {
    int n, k;
    std::cin >> n >> k;
    BinaryIndexedTree binaryIndexedTree(n);
    for (int i = 0; i < k; ++i) {
        char type;
        std::cin >> type;
        switch (type) {
            case 'A':
                int index, value;
                std::cin >> index >> value;
                binaryIndexedTree.ChangeI(index - 1, value);
                break;
            case 'Q':
                int start, end;
                std::cin >> start >> end;
                std::cout << binaryIndexedTree.CalcSum(start - 1, end - 1) << std::endl;
                break;
        }
    }
    return 0;
}