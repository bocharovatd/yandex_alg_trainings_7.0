// H. Звезды

// Вася любит наблюдать за звёздами. Но следить за всем небом сразу ему тяжело. 
// Поэтому он наблюдает только за частью пространства, ограниченной кубом размером n×n×n. 
// Этот куб поделен на маленькие кубики размером 1×1×1. 
// Во время его наблюдений могут происходить следующие события:

// В каком-то кубике появляются или исчезают несколько звёзд.
// К нему может заглянуть его друг Петя и поинтересоваться, сколько видно звёзд в части пространства, 
// состоящей из нескольких кубиков.
// Формат ввода
// Первая строка входного файла содержит натуральное число 1≤n≤128. Координаты кубиков — целые числа от 0 до n−1. 
// Далее следуют записи о происходивших событиях по одной в строке. В начале строки записано число m. 
// Если m равно:

// 1. то за ним следуют 4 числа — x, y, z (0≤x,y,z<N) и k (−20000≤k≤20000) — координаты кубика и величина, 
// на которую в нём изменилось количество видимых звёзд;
// 2. то за ним следуют 6 чисел — x1, y1, z1, x2, y2, z2 (0≤x1≤x2<N, 0≤y1≤y2<N, 0≤z1≤z2<N), которые означают, 
// что Петя попросил подсчитать количество звёзд в кубиках (x,y,z) из области: x1≤x≤x2, y1≤y≤y2, z1≤z≤z2;
// 3. то это означает, что Васе надоело наблюдать за звёздами и отвечать на вопросы Пети. 
// Эта запись встречается во входном файле только один раз и будет последней.
// Количество записей во входном файле не больше 100002.

// Формат вывода
// Для каждого Петиного вопроса выведите искомое количество звезд.

#include <iostream>

struct Point {
    int x;
    int y;
    int z;
};

class BinaryIndexedTree {
    public:
    BinaryIndexedTree(int n);
    ~BinaryIndexedTree();
    void ChangeXYZ(Point point, int value);
    unsigned long long CalcSum(Point start, Point end);
    private:
    unsigned long long*** binaryIndexTree;
    int binaryIndexTreeSize;
    unsigned long long calcPrefixSum(Point point);
};

BinaryIndexedTree::BinaryIndexedTree(int n) {
    binaryIndexTree = new unsigned long long**[n + 1]; 
    for (int i = 0; i < n + 1; ++i) {
        binaryIndexTree[i] = new unsigned long long*[n + 1];
        for (int j = 0; j < n + 1; ++j) {
            binaryIndexTree[i][j] = new unsigned long long[n + 1]();
        }
    }
    binaryIndexTreeSize = n;
}

BinaryIndexedTree::~BinaryIndexedTree() {
    for (int i = 0; i < binaryIndexTreeSize + 1; ++i) {
        for (int j = 0; j < binaryIndexTreeSize + 1; ++j) {
            delete[] binaryIndexTree[i][j];
        }
        delete[] binaryIndexTree[i];
    }
    delete[] binaryIndexTree;
}

unsigned long long BinaryIndexedTree::CalcSum(Point start, Point end) {
    unsigned long long result = 
        calcPrefixSum(end) 
        - calcPrefixSum(Point{start.x - 1, end.y, end.z})
        - calcPrefixSum(Point{end.x, start.y - 1, end.z})
        - calcPrefixSum(Point{end.x, end.y, start.z - 1})
        + calcPrefixSum(Point{start.x - 1 + (end.x - start.x + 1), start.y - 1, start.z - 1})
        + calcPrefixSum(Point{start.x - 1, start.y - 1 + (end.y - start.y + 1), start.z - 1})
        + calcPrefixSum(Point{start.x - 1, start.y - 1, start.z - 1 + (end.z - start.z + 1)})
        - calcPrefixSum(Point{start.x - 1, start.y - 1, start.z - 1});
    return result;
}

unsigned long long BinaryIndexedTree::calcPrefixSum(Point point) {
    unsigned long long sum = 0;
    int x = point.x;
    do {
        int y = point.y;
        do {
            int z = point.z;
            do {
                sum += binaryIndexTree[x][y][z];
                z = (z & (z + 1)) - 1;
            } while (z > 0);
            y = (y & (y + 1)) - 1;
        } while (y > 0);
        x = (x & (x + 1)) - 1;
    } while (x > 0);
    return sum;
}

void BinaryIndexedTree::ChangeXYZ(Point point, int value) {
    for (int x = point.x; x < binaryIndexTreeSize + 1;) {
        for (int y = point.y; y < binaryIndexTreeSize + 1;) {
            for (int z = point.z; z < binaryIndexTreeSize + 1;) {
                binaryIndexTree[x][y][z] += value;
                z = z | (z + 1);
            }
            y = y | (y + 1);
        }
        x = x | (x + 1);
    }
}

int main() {
    int n;
    std::cin >> n;
    BinaryIndexedTree binaryIndexedTree(n);
    char type = ' ';
    while (type != '3') {
        std::cin >> type;
        switch (type) {
            case '1':
                Point point;
                int value;
                std::cin >> point.x >> point.y >> point.z >> value;
                ++point.x;
                ++point.y;
                ++point.z;
                binaryIndexedTree.ChangeXYZ(point, value);
                break;
            case '2':
                Point start, end;
                std::cin >> start.x >> start.y >> start.z >> end.x >> end.y >> end.z;
                ++start.x;
                ++start.y;
                ++start.z;
                ++end.x;
                ++end.y;
                ++end.z;
                std::cout << binaryIndexedTree.CalcSum(start, end) << std::endl;
                break;
        }
    }
    return 0;
}