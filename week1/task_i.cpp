
// I. Эластичный ровер*

// Экспериментальная модель ровера обладает эластичным отсеком для перевозки заказов.

// Базовый объём отсека составляет S литров. Пока отсек не заполнен, товары в нём не испытывают 
// дополнительного давления. Однако, поскольку отсек эластичный, в него можно положить дополнительные 
// товары сверх базового объёма. Если объём положенных в отсек товаров U превышает S, то все товары 
// в отсеке будут испытывать давление P = U - S.

// Каждый товар обладает тремя характеристиками:
// - объёмом v_i
// - стоимостью c_i 
// - давлением, которое он выдерживает p_i

// Всего необходимо доставить N товаров, однако в первую поездку ровера необходимо отправить товары 
// с максимальной суммарной стоимостью - это обрадует заказчика. Определите максимальную стоимость 
// товаров, которые можно разместить в ровере так, чтобы все они выдерживали давление.

// Формат ввода:
// В первой строке находятся два целых числа N (1 ≤ N ≤ 100) и S (0 ≤ S ≤ 10^9) - количество товаров.
// Следующие N строк содержат описание очередного товара: v_i, c_i и p_i 
// (1 ≤ v_i ≤ 1000, 0 ≤ c_i ≤ 10^6, 0 ≤ p_i ≤ 10^9)

// Формат вывода:
// В первой строке выведите число товаров K, которые необходимо разместить в отсеке ровера, 
// и их максимальную суммарную стоимость.
// Во второй строке выведите K чисел - номера товаров, которые нужно разместить в отсеке ровера.
// Товары нумеруются с единицы в том порядке, в котором они даны во входном файле.
// Если существует несколько вариантов ответа, выведите любой из них.

#include <iostream>
#include <algorithm>
# include <vector>

struct Product {
    int n;
    int v;
    int c;
    int p;

    bool operator>(const Product& other) const {
        return p > other.p;
    }
};

struct Arr_resore_item {
    int index;
    int c_sum;
};

Arr_resore_item max_item(Arr_resore_item item_1, Arr_resore_item item_2) {
    if (item_1.c_sum >= item_2.c_sum) {
        return item_1;
    }
    return item_2;
}

void copy_i1_to_i2(Arr_resore_item** arr_item, int m, int i1, int i2) {
    for (int j = 0; j < m; ++j) {
        arr_item[i2][j] = arr_item[i1][j];
    }
}

void find_max_c_with_answer(Product* arr_prod, int n, int s, int extra_v) {
    Arr_resore_item** arr_restore_answer = new Arr_resore_item*[n];
    for (int i = 0; i < n; ++i) {
        arr_restore_answer[i] = new Arr_resore_item[s + extra_v + 1];
    }
    for (int i = 0; i < n; ++i) {
        arr_restore_answer[i][0].index = -1;
        arr_restore_answer[i][0].c_sum = 0;
        for (int j = 1; j < s + extra_v + 1; ++j) {
            arr_restore_answer[i][j].index = -1;
            arr_restore_answer[i][j].c_sum = -1;
        }
    }
    for (int i = 0; i < n; ++i) {
        int v = arr_prod[i].v;
        int c = arr_prod[i].c;
        int p = arr_prod[i].p;
        for (int j = s + extra_v - v; j >= 0; --j) {
            if (arr_restore_answer[i][j].c_sum != -1) {
                if (j + v - s <= p && j + v <= s + extra_v) {
                    int new_c = arr_restore_answer[i][j].c_sum + c;
                    Arr_resore_item new_item = { .index = i, .c_sum = new_c };
                    arr_restore_answer[i][j + v] = max_item(arr_restore_answer[i][j + v], new_item);
                }
            }
        }
        if (i + 1 != n) {
            copy_i1_to_i2(arr_restore_answer, s + extra_v + 1, i, i + 1);
        }
    }
    int mx_c = 0;
    int mx_c_j = 0;
    for (int j = s + extra_v; j >= 0; --j) {
        if (arr_restore_answer[n - 1][j].c_sum > mx_c) {
            mx_c = arr_restore_answer[n - 1][j].c_sum;
            mx_c_j = j;
        }
    }
    int cur_c = 0;
    int cur_j = mx_c_j;
    int cur_i = n - 1;
    std::vector<int> arr_answer;
    while (cur_c != mx_c) {
        arr_answer.push_back(arr_prod[arr_restore_answer[cur_i][cur_j].index].n);
        cur_c += arr_prod[arr_restore_answer[cur_i][cur_j].index].c;
        int new_cur_j = cur_j - arr_prod[arr_restore_answer[cur_i][cur_j].index].v;
        int new_cur_i = arr_restore_answer[cur_i][cur_j].index - 1;
        cur_i = new_cur_i;
        cur_j = new_cur_j;
    }

    std::cout << arr_answer.size() << " " << mx_c << std::endl;
    for (auto i = arr_answer.begin(); i != arr_answer.end(); ++i) {
        std::cout << *i << std::endl;
    }

    for (int i = 0; i < n; ++i) {
        delete[] arr_restore_answer[i];
    }
    delete[] arr_restore_answer;
}

int main() {
    int n, s;
    std::cin >> n >> s;
    int v_sum = 0;
    Product* arr_prod = new Product[n];
    for (int i = 0; i < n; ++i) {
        int v, c, p;
        std::cin >> v >> c >> p;
        v_sum += v;
        Product prod = { 
            .n = i + 1,
            .v = v,
            .c = c,
            .p = p 
        };
        arr_prod[i] = prod;
    }
    if (s > v_sum) {
        s = v_sum;
    }
    std::sort(arr_prod, arr_prod + n, std::greater<Product>());
    int extra_v = arr_prod[0].p;
    if (s + extra_v > v_sum) {
        extra_v = v_sum - s;
    }
    find_max_c_with_answer(arr_prod, n, s, extra_v);
    delete[] arr_prod;
    return 0;
}
