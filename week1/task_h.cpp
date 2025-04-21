// H. Распределение задач*

// Вася и Маша организовали производство бирдекелей. Они договорились работать день через день, 
// при этом Вася любит простую и понятную работу, а Маша — сложную и творческую. 
// В первый день работает Вася, во второй — Маша, потом снова Вася и т.д.

// К ним поступило N заказов, для каждого заказа известна его продолжительность в днях 
// и для каждого из дней известно, будет ли в этот день работа сложной или простой. 
// Заказы можно выполнять в любом порядке, перерывов между заказами нет.

// Определите такой порядок выполнения заказов, чтобы Вася получил как можно больше простых задач, 
// а Маша — сложных.

// Формат ввода:
// В первой строке задается число N (1 ≤ N ≤ 100 000) — количество заказов.
// В следующих N строках описываются заказы. Каждое описание представляет собой строку, 
// состоящую из символов S и D, обозначающих, соответственно, простую и сложную работу в этот день. 
// Длина строки не превосходит 100 символов. Суммарная длина всех строк не превосходит 1 000 000 символов.

// Формат вывода:
// Выведите одно число — максимальное количество дней с простой работой, которые достанутся Васе.

# include <iostream>
# include <string>
#include <algorithm>

int pos_rem(int a, int b) {
    return  (a % b + b) % b;
}

struct Task {
    int s_even;
    int diff_s_even_odd;
    bool changes_sign;

    bool operator<(const Task& other) const {
        return diff_s_even_odd < other.diff_s_even_odd;
    }
};

Task convert_to_task(std::string str) {
    int s_even = 0;
    int s_odd = 0;
    for (int i = 0; i < str.length(); ++i) {
        if (str[i] == 'S') {
            if (i % 2 == 0) {
                ++s_even;
            } else {
                ++s_odd;
            }
        }
    }
    Task task = {
        .s_even = s_even, 
        .diff_s_even_odd = s_even - s_odd,
        .changes_sign = (str.length() % 2 != 0) ? true : false 
    };
    return task;
}

std::pair<int, Task> merge_tasks_not_changes_sign(Task* arr_tasks, int n, bool diff_is_pos) {
    int s_even = 0;
    int diff_s_even_odd = 0;
    int count = 0;
    for (int i = 0; i < n; ++i) {
        if (!arr_tasks[i].changes_sign) {
            if (arr_tasks[i].diff_s_even_odd >= 0 && diff_is_pos || 
                arr_tasks[i].diff_s_even_odd < 0 && !diff_is_pos) {
                    s_even += arr_tasks[i].s_even;
                    diff_s_even_odd += arr_tasks[i].diff_s_even_odd;
                    ++count;
                }
        }
    }
    Task merged_task = {
        .s_even = s_even, 
        .diff_s_even_odd = diff_s_even_odd,
        .changes_sign = false
    };
    return std::make_pair(count, merged_task);
}

Task* create_arr_changes_sign(Task* arr_tasks, int n, int n_changes_sign) {
    Task* arr_tasks_changes_sign = new Task[n_changes_sign];
    int cur_i = 0;
    for (int i = 0; i < n; ++i) {
        if (arr_tasks[i].changes_sign) {
            arr_tasks_changes_sign[cur_i++] = arr_tasks[i];
        }
    }
    return arr_tasks_changes_sign;
}

int find_min_days_s(Task* arr_tasks, int n, Task task_peek_first_even, Task task_peek_first_odd) {
    int result = 0;
    result += task_peek_first_even.s_even;
    int count = 0;
    int i = n - 1;
    while (count != n) {
        if (count == 1) {
            result += task_peek_first_odd.s_even - task_peek_first_odd.diff_s_even_odd;
        }
        if (count % 2 == 0) {
            result += arr_tasks[i].s_even;
            ++count;
            i = pos_rem(i + count, n);
        } else {
            result += arr_tasks[i].s_even - arr_tasks[i].diff_s_even_odd;
            ++count;
            i = pos_rem(i - count, n);
        }
    }
    if (count < 1) {
        result += task_peek_first_odd.s_even;
    }
    return result;
}

int main() {
    int n;
    std::cin >> n;
    Task* arr_tasks = new Task[n];
    for (int i = 0; i < n; ++i) {
        std::string str;
        std::cin >> str;
        arr_tasks[i] = convert_to_task(str);
    }

    int count_peek_first_even, count_peek_first_odd;
    Task task_peek_first_even, task_peek_first_odd;
    std::pair<int, Task> result;

    result = merge_tasks_not_changes_sign(arr_tasks, n, true);
    count_peek_first_even = result.first;
    task_peek_first_even = result.second;

    result = merge_tasks_not_changes_sign(arr_tasks, n, false);
    count_peek_first_odd = result.first;
    task_peek_first_odd = result.second;

    int n_changes_sign = n - count_peek_first_even - count_peek_first_odd;
    Task* arr_tasks_changes_sign = create_arr_changes_sign(arr_tasks, n, n_changes_sign);
    std::sort(arr_tasks_changes_sign, arr_tasks_changes_sign + n_changes_sign);
    
    delete[] arr_tasks;

    std::cout << find_min_days_s(arr_tasks_changes_sign, n_changes_sign, task_peek_first_even, task_peek_first_odd);

    delete[] arr_tasks_changes_sign;
    return 0;
}