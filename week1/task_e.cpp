// E. Рюкзак: наибольшая стоимость

// Дано N предметов массой m1, m2, …, mN и стоимостью c1, c2, …, cN, соответственно.
// Ими наполняют рюкзак, который выдерживает вес не более M.
// Какую наибольшую стоимость могут иметь предметы в рюкзаке?

// Формат ввода:
// В первой строке вводится натуральное число N, не превышающее 100, и натуральное число M, не превышающее 10000.
// Во второй строке вводятся N натуральных чисел mi, не превышающих 100.
// В третьей строке вводятся N натуральных чисел ci, не превышающих 100.

// Формат вывода:
// Выведите наибольшую стоимость рюкзака.

#include <iostream>

void find_carry_max(int* arr_mass, int* arr_cost, int n, int m) {
    int* arr_check = new int[m + 1];
    arr_check[0] = 0;
    for (int i = 1; i < m + 1; ++i) {
        arr_check[i] = -1;
    }
    for (int i = 0; i < n; ++i) {
        int mass = arr_mass[i];
        int cost = arr_cost[i];
        for (int j = m - mass; j >= 0; --j) {
            if (arr_check[j] >= 0) {
                if (arr_check[j] + cost > arr_check[j + mass]) {
                    arr_check[j + mass] = arr_check[j] + cost;
                }
            }
        }
    }
    int mx_cost = 0;
    for (int j = m; j >= 0; --j) {
        if (arr_check[j] > mx_cost) {
            mx_cost = arr_check[j];
        }
    }
    std::cout << mx_cost;
    delete[] arr_check;
}

int main() {
    int n, m;
    std::cin >> n >> m;
    int* arr_mass = new int[n];
    int* arr_cost = new int[n];
    for (int i = 0; i < n; ++i) {
        std::cin >> arr_mass[i];
    }
    for (int i = 0; i < n; ++i) {
        std::cin >> arr_cost[i];
    }
    find_carry_max(arr_mass, arr_cost, n, m);
    delete[] arr_mass;
    delete[] arr_cost;
    return 0;
}