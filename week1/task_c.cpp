// C. Интернет

// Новый интернет-провайдер предоставляет услугу доступа в интернет с посекундной тарификацией.
// Для подключения нужно купить карточку, позволяющую пользоваться интернетом определенное количество секунд.
// При этом компания продает карточки стоимостью 1, 2, 4, …, 2^30 рублей на a0, a1, a2, …, a30 секунд соответственно.

// Родители разрешили Пете пользоваться интернетом M секунд. Определите, за какую наименьшую сумму он сможет купить карточки,
// которые позволят ему пользоваться интернетом не менее M секунд. 
// Естественно, Петя может купить как карточки различного достоинства,
// так и несколько карточек одного достоинства.

// Формат ввода:
// В первой строке содержится единственное натуральное число M (1 ≤ M ≤ 10^9).
// Во второй строке задаются натуральные числа a0, a1, …, a30, не превосходящие 10^9.

// Формат вывода:
// Выведите единственное число — наименьшую сумму денег, которую Пете придется потратить.

#include <iostream>
#include <math.h>
#include <vector>

std::vector<int> maximize_minutes(int* arr_mins, int n, int target_mins) {
    std::vector<int> arr_optimized_mins;
    arr_optimized_mins.push_back(arr_mins[0]);
    for (int i = 1; i < n; ++i) {
        int optimized_mins = std::max(arr_optimized_mins[i - 1] * 2, arr_mins[i]);
        arr_optimized_mins.push_back(optimized_mins);
        if (optimized_mins >= target_mins) {
            break;
        }
    }
    int i = arr_optimized_mins.size() - 1;
    while (arr_optimized_mins[i] < target_mins) {
        arr_optimized_mins.push_back(arr_optimized_mins[i] * 2);
        ++i;
    }
    return arr_optimized_mins;
}

int find_min_cost(std::vector<int> arr_mins, int n, int target_mins) {
    int larger_i = n - 1;
    if (arr_mins[larger_i] == target_mins) {
        return std::pow(2, larger_i);
    }
    int result = std::pow(2, larger_i);
    int new_result = 0;
    int sum_mins = 0;
    for (int i = larger_i - 1; i >= 0; --i) {
        sum_mins += arr_mins[i];
        new_result += std::pow(2, i);
        if (sum_mins >= target_mins) {
            result = std::min(result, new_result);
            sum_mins -= arr_mins[i];
            new_result -= std::pow(2, i);
        }
    }
    return result;
}

int main() {
    int n = 30;
    int target_mins;
    std::cin >> target_mins;
    int* arr_mins = new int[n];
    for (int i = 0; i < n; ++i) {
        std::cin >> arr_mins[i];
    }
    std::vector<int> arr_optimized_mins = maximize_minutes(arr_mins, n, target_mins);
    std::cout << find_min_cost(arr_optimized_mins, arr_optimized_mins.size(), target_mins);
    delete[] arr_mins;
    return 0;
}