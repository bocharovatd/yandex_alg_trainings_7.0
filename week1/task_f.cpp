// F. Рюкзак: наибольшая стоимость с восстановлением ответа

// Дано N предметов массой m1, m2, …, mN и стоимостью c1, c2, …, cN, соответственно.
// Ими наполняют рюкзак, который выдерживает вес не более M.
// Определите набор предметов, который можно унести в рюкзаке, имеющий наибольшую стоимость.

// Формат ввода:
// В первой строке вводится натуральное число N, не превышающее 100, и натуральное число M, не превышающее 10000.
// Во второй строке вводятся N натуральных чисел mi, не превышающих 100.
// В третьей строке вводятся N натуральных чисел ci, не превышающих 100.

// Формат вывода:
// Выведите номера предметов (числа от 1 до N), которые войдут в рюкзак наибольшей стоимости.

#include <iostream>

std::pair<int, int> max_in_pair(std::pair<int, int> p_1, std::pair<int, int> p_2) {
    if (p_1.second >= p_2.second) {
        return p_1;
    }
    return p_2;
}

void copy_i1_to_i2(std::pair<int, int>** arr_p, int m, int i1, int i2) {
    for (int j = 0; j < m; ++j) {
        arr_p[i2][j] = arr_p[i1][j];
    }
}

void find_max_c_with_answer(int* arr_mass, int* arr_cost, int n, int m) {
    std::pair<int, int>** arr_restore_answer = new std::pair<int, int>*[n];
    for (int i = 0; i < n; ++i) {
        arr_restore_answer[i] = new std::pair<int, int>[m + 1];
    }
    for (int i = 0; i < n; ++i) {
        arr_restore_answer[i][0].first = -1;
        arr_restore_answer[i][0].second = 0;
        for (int j = 1; j < m + 1; ++j) {
            arr_restore_answer[i][j].first = -1;
            arr_restore_answer[i][j].second = -1;
        }
    }
    for (int i = 0; i < n; ++i) {
        int mass = arr_mass[i];
        int cost = arr_cost[i];
        for (int j = m - mass; j >= 0; --j) {
            if (arr_restore_answer[i][j].second != -1) {
                int new_cost = arr_restore_answer[i][j].second + cost;
                arr_restore_answer[i][j + mass] = max_in_pair(arr_restore_answer[i][j + mass], std::make_pair(i, new_cost));
            }
        }
        if (i + 1 != n) {
            copy_i1_to_i2(arr_restore_answer, m + 1, i, i + 1);
        }
    }
    int mx_cost = 0;
    int mx_cost_j = 0;
    for (int j = m; j >= 0; --j) {
        if (arr_restore_answer[n - 1][j].second > mx_cost) {
            mx_cost = arr_restore_answer[n - 1][j].second;
            mx_cost_j = j;
        }
    }
    int cur_cost = 0;
    int cur_j = mx_cost_j;
    int cur_i = n - 1;
    while (cur_cost != mx_cost) {
        std::cout << arr_restore_answer[cur_i][cur_j].first + 1 << std::endl;
        cur_cost += arr_cost[arr_restore_answer[cur_i][cur_j].first];
        int new_cur_j = cur_j - arr_mass[arr_restore_answer[cur_i][cur_j].first];
        int new_cur_i = arr_restore_answer[cur_i][cur_j].first - 1;
        cur_i = new_cur_i;
        cur_j = new_cur_j;
    }

    for (int i = 0; i < n; ++i) {
        delete[] arr_restore_answer[i];
    }
    delete[] arr_restore_answer;
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
    find_max_c_with_answer(arr_mass, arr_cost, n, m);
    delete[] arr_mass;
    delete[] arr_cost;
    return 0;
}