// D. Рюкзак: наибольший вес

// Дано N золотых слитков массой m1, m2, …, mN. Ими наполняют рюкзак, который выдерживает вес не более M.
// Какую наибольшую массу золота можно унести в таком рюкзаке?

// Формат ввода:
// В первой строке вводится натуральное число N, не превышающее 100, и натуральное число M, не превышающее 10000.
// Во второй строке вводятся N натуральных чисел mi, не превышающих 100.

// Формат вывода:
// Выведите одно целое число — наибольшую возможную массу золота, которую можно унести в данном рюкзаке.

#include <iostream>

void find_carry_max(int* arr_bar, int n, int m) {
    bool* arr_check = new bool[m + 1]();
    arr_check[0] = true;
    for (int i = 0; i < n; ++i) {
        int bar = arr_bar[i];
        for (int j = m - bar; j >= 0; --j) {
            if (arr_check[j]) {
                arr_check[j + bar] = true;
            }
        }
    }
    for (int j = m; j >= 0; --j) {
        if (arr_check[j]) {
            std::cout << j;
            break;
        }
    }
    delete[] arr_check;
}

int main() {
    int n, m;
    std::cin >> n >> m;
    int* arr_bar = new int[n];
    for (int i = 0; i < n; ++i) {
        std::cin >> arr_bar[i];
    }
    find_carry_max(arr_bar, n, m);
    delete[] arr_bar;
    return 0;
}