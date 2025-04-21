// A. Количество максимумов на отрезке

// Реализуйте структуру данных для эффективного вычисления максимального значения 
// из нескольких подряд идущих элементов массива, а также количества элементов, 
// равных максимальному на данном отрезке.

// Формат ввода
// В первой строке вводится одно натуральное число 
// N (1 ≤ N ≤ 100 000 1≤N≤100000)  — количество чисел в массиве.

// Во второй строке вводятся
// N чисел от 1 до 100 000 — элементы массива.

// В третьей строке вводится одно натуральное число 
// K ( 1 ≤ K ≤ 30 000) — количество запросов на вычисление максимума.

// В следующих 
// K строках вводится по два числа — номера левого и правого элементов отрезка массива 
// (считается, что элементы массива нумеруются с единицы).

// Формат вывода
// Для каждого запроса выведите в отдельной строке через пробел значение максимального элемента 
// на указанном отрезке массива и количество максимальных элементов на этом отрезке.

#include <iostream>
#include <math.h>
#include <map>

class SparceTable {
    public:
        SparceTable (int n);
        ~SparceTable();
        void Fill(int* arr, int n);
        int FindMax(int a, int b);
        int FindCount(int a, int b, int mx);
    private:
        struct Elem {
            int mx;
            int count;
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
        matrix[0][j].count = 1;
    }
    for (int i = 1; i < rowsCount; ++i) {
        int seg = pow(2, i);
        int prev_seg = pow(2, i - 1);
        for (int j = 0; j + seg <= colsCount; ++j) {
            Elem elemLeft = matrix[i - 1][j];
            Elem elemRight = matrix[i - 1][j + prev_seg];
            if (elemLeft.mx > elemRight.mx) {
                matrix[i][j] = elemLeft;
            } else if (elemLeft.mx < elemRight.mx) {
                matrix[i][j] = elemRight;
            } else {
                matrix[i][j].mx = elemLeft.mx;
                matrix[i][j].count = elemLeft.count + elemRight.count;
            }
        }
    }
}

int SparceTable::FindMax(int a, int b) {
    int i = mapTwoPowK[b - a + 1];
    int seg = pow(2, i);
    return std::max(matrix[i][a - 1].mx, matrix[i][b - seg].mx);
}

int SparceTable::FindCount(int a, int b, int mx) {
    int i = mapTwoPowK[b - a + 1];
    int seg = pow(2, i);
    int segIntersec = (a + seg - 1) - (b - seg + 1) + 1;
    int resultCount = 0;
    if (mx == matrix[i][a - 1].mx) {
        resultCount += matrix[i][a - 1].count;
    }
    if (mx == matrix[i][b - seg].mx) {
        resultCount += matrix[i][b - seg].count;
    }
    if (mx == FindMax(b - seg + 1, a + seg - 1)) {
        int iIntersec = mapTwoPowK[segIntersec];
        if (segIntersec == pow(2, iIntersec)) {
            resultCount -= matrix[iIntersec][b - seg].count;
        } else {
            resultCount -= FindCount(b - seg + 1, a + seg - 1, mx);
        }
    }
    return resultCount;
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
        int resultCount = sparceTable.FindCount(segs[i].first, segs[i].second, resultMx);
        std::cout << resultMx << " " << resultCount << std::endl;
    }
    delete[] arr;
    delete[] segs;
}