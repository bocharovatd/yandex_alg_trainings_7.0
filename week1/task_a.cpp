// A. Каждому по компьютеру

// В новом учебном году во Дворец Творчества Юных для занятий в компьютерных классах пришли учащиеся, 
// которые были разбиты на N групп.
// В i-й группе оказалось Xi человек. Тут же перед директором встала серьезная проблема: 
// как распределить группы по аудиториям.
// Во дворце имеется M ≥ N аудиторий, в j-й аудитории имеется Yj компьютеров.

// Для занятий необходимо, чтобы у каждого учащегося был компьютер и еще один компьютер был у преподавателя.
// Переносить компьютеры из одной аудитории в другую запрещается.
// Помогите директору!

// Напишите программу для поиска максимального количества групп, 
// которое удастся одновременно распределить по аудиториям,
// чтобы всем учащимся в каждой группе хватило компьютеров, и при этом остался хотя бы один для учителя.

// Формат ввода:
// На первой строке входного файла расположены числа N и M (1 ≤ N ≤ M ≤ 1000).
// На второй строке расположены N чисел — X1, X2, ..., XN (1 ≤ Xi ≤ 1000 для всех 1 ≤ i ≤ N).
// На третьей строке расположено M чисел — Y1, Y2, ..., YM (1 ≤ Yi ≤ 1000 для всех 1 ≤ i ≤ M).

// Формат вывода:
// Выведите на первой строке число P — количество групп, которое удастся распределить по аудиториям.
// На второй строке выведите распределение групп по аудиториям — N чисел, 
// i-е число должно соответствовать номеру аудитории,
// в которой должна заниматься i-я группа. Нумерация как групп, так и аудиторий, начинается с 1.
// Если i-я группа осталась без аудитории, i-е число должно быть равно 0.
// Если допустимых распределений несколько, выведите любое из них.

#include <iostream>
#include <map>

int partition(std::pair<int, int>* arr, int n) {
    if (n <= 1) {
        return 0;
    }
    std::pair<int, int> pivot = arr[n - 1];
    int i = 0;
    int j = n - 1 - 1;
    while (i <= j) {
        while (arr[i].second < pivot.second) {
            ++i;
        }
        while (j >= 0 && arr[j].second >= pivot.second) {
            --j;
        }
        if (i < j) {
            std::swap(arr[i++], arr[j--]);
        }
    }
    std::swap(arr[i], arr[n - 1]);
    return i;
}

void quick_sort(std::pair<int, int>* arr, int n) {
    int part = partition(arr, n);
    if (part > 0) {
        quick_sort(arr, part);
    }
    if (part + 1 < n) {
        quick_sort(arr + part + 1, n - (part + 1));
    }
}

void map_arr_x_arr_y(std::pair<int, int>* arr_x, int n, std::pair<int, int>* arr_y, int m) {
    int* result = new int[n];
    int j = m - 1;
    for (int i = n - 1; i >= 0; --i) {
        if (arr_x[i].second + 1 <= arr_y[j].second) {
            result[arr_x[i].first] = arr_y[j].first + 1;
            --j;
        } else {
            result[arr_x[i].first] = 0;
        }
    }
    std::cout << m - j - 1 << std::endl;
    for (int i = 0; i < n; ++i) {
        std::cout << result[i] << " ";
    }
    delete[] result;
}

int main() {
    int n, m;
    std::cin >> n >> m;
    std::pair<int, int>* arr_x = new std::pair<int, int>[n];
    std::pair<int, int>* arr_y = new std::pair<int, int>[m];
    for (int i = 0; i < n; ++i) {
        int x_i;
        std::cin >> x_i;
        arr_x[i] = std::make_pair(i, x_i);
    }
    for (int i = 0; i < m; ++i) {
        int y_i;
        std::cin >> y_i;
        arr_y[i] = std::make_pair(i, y_i);
    }
    quick_sort(arr_x, n);
    quick_sort(arr_y, m);

    map_arr_x_arr_y(arr_x, n, arr_y, m);

    delete[] arr_x;
    delete[] arr_y;

    return 0;
}