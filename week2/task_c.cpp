// C. Максимум и индекс максимума на отрезке

// Реализуйте структуру данных, которая на данном массиве из N целых чисел позволяет узнать
// максимальное значение на этом массиве и индекс элемента, на котором достигается это максимальное значение.
//
// Формат ввода:
// В первой строке вводится натуральное число N (1 ≤ N ≤ 10^5) - количество элементов в массиве.
// Во второй строке содержатся N целых чисел, не превосходящих по модулю 10^9 - элементы массива.
// Гарантируется, что в массиве нет одинаковых элементов.
// В третьей строке идет число K (0 ≤ K ≤ 10^5) - количество запросов к структуре данных.
// Каждая из следующих K строк содержит два целых числа l и r (1 ≤ l ≤ r ≤ N) - левую и правую границы отрезка.
//
// Формат вывода:
// Для каждого из запросов выведите два числа:
// наибольшее значение среди элементов массива на отрезке от l до r
// индекс одного из элементов массива, принадлежащий отрезку от l до r, на котором достигается этот максимум.

#include <iostream>
#include <math.h>
#include <map>

class SparceTable {
    public:
        SparceTable (int n);
        ~SparceTable();
        void Fill(int* arr, int n);
        int FindMax(int a, int b);
        int FindIndexMx(int a, int b, int mx);
    private:
        struct Elem {
            int mx;
            int indexMx;
        };
        Elem** matrix;
        int rowsCount;
        int colsCount;
        int* mapTwoPowK;
};

SparceTable::SparceTable(int n) {
    mapTwoPowK = new int[n + 1];
    int i = 0;
    int two_pow_i = 1;
    int k = 1;
    while (k <= n) {
        while (two_pow_i * 2 > k && k <= n) {
            mapTwoPowK[k] = i;
            ++k;
        }
        two_pow_i *= 2;
        ++i;
    }

    rowsCount = mapTwoPowK[n] + 1;
    colsCount = n;
    matrix = new Elem*[rowsCount];
    for (int i = 0; i < rowsCount; ++i) {
        matrix[i] = new Elem[colsCount];
    }
}

SparceTable::~SparceTable() {
    for (int i = 0; i < rowsCount; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;

    delete[] mapTwoPowK;
}

void SparceTable::Fill(int* arr, int n) {
    for (int j = 0; j < colsCount; ++j) {
        matrix[0][j].mx = arr[j];
        matrix[0][j].indexMx = j;
    }
    for (int i = 1; i < rowsCount; ++i) {
        int seg = pow(2, i);
        int prev_seg = pow(2, i - 1);
        for (int j = 0; j + seg <= colsCount; ++j) {
            Elem elemLeft = matrix[i - 1][j];
            Elem elemRight = matrix[i - 1][j + prev_seg];
            if (elemLeft.mx >= elemRight.mx) {
                matrix[i][j] = elemLeft;
            } else {
                matrix[i][j] = elemRight;
            }
        }
    }
}

int SparceTable::FindMax(int a, int b) {
    int i = mapTwoPowK[b - a + 1];
    int seg = pow(2, i);
    return std::max(matrix[i][a - 1].mx, matrix[i][b - seg].mx);
}

int SparceTable::FindIndexMx(int a, int b, int mx) {
    int i = mapTwoPowK[b - a + 1];
    int seg = pow(2, i);
    if (mx == matrix[i][a - 1].mx) {
        return matrix[i][a - 1].indexMx;
    }
    return matrix[i][b - seg].indexMx;
}

int main() {
    int n;
    std::cin >> n;
    int* arr = new int[n];
    for (int i = 0; i < n; ++i) {
        std::cin >> arr[i];
    }

    int k;
    std::cin >> k;
    std::pair<int, int>* segs = new std::pair<int, int>[k];
    for (int i = 0; i < k; ++i) {
        int start, end;
        std::cin >> start >> end;
        segs[i] = std::make_pair(start, end);
    }

    SparceTable sparceTable(n);
    sparceTable.Fill(arr, n);
    for (int i = 0; i < k; ++i) {
        int resultMx = sparceTable.FindMax(segs[i].first, segs[i].second);
        int resultIndexMx = sparceTable.FindIndexMx(segs[i].first, segs[i].second, resultMx);
        std::cout << resultMx << " " << resultIndexMx + 1 << std::endl;
    }
    delete[] arr;
    delete[] segs;
}