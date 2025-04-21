// G. Две стены

// У Пети есть набор из N кирпичиков. Каждый кирпичик полностью окрашен в один из K цветов,
// i-й кирпичик имеет размер 1×1×L_i. Петя знает, что он может построить из кирпичиков
// прямоугольную стену толщиной 1 и высотой K, причем первый горизонтальный слой кирпичиков
// в стене будет первого цвета, второй — второго и т. д. Теперь Петя хочет узнать, может ли он
// из своего набора построить две прямоугольные стены, обладающие тем же свойством. Помогите ему выяснить это.

// Формат ввода:
// В первой строке входных данных задаются числа N и K (1 <= N <= 5000, 1 <= K <= 100).
// Следующие N строк содержат описание Петиных кирпичиков: сначала длина L_i, затем номер цвета C_i 
// (1 <= L_i <= 100, 1 <= C_i <= K). Известно, что у Пети не более 50 кирпичиков каждого цвета.

// Формат вывода:
// Выведите в первой строке YES, если Петя сможет построить из своих кирпичиков две прямоугольные
// стены высоты K, j-й слой кирпичиков в каждой из которых будет j-го цвета, и NO в противном случае.
// В случае положительного ответа, выведите во второй строке в произвольном порядке номера кирпичиков,
// из которых следует построить первую стену (кирпичики нумеруются в том порядке, в котором они заданы
// во входных данных, начиная с 1). Если решений несколько, можно выдать любое из них.

#include <iostream>

struct Brick {
    int num;
    int length;
};

int find_possible_one_wall_length(int** arrs_with_length_options, int k, int layer_length) {
    for (int j = 1; j < layer_length; ++j) {
        bool flag = true;
        for (int i = 0; i < k; ++i) {
            if (arrs_with_length_options[i][j] == -1) {
                flag = false;
                break;
            }
        }
        if (flag) {
            return j;
        }
    }
    return 0;
}

void restore_answer(int* arr_with_length_options, Brick* arr_bricks, int target_length) {
    int cur_length = 0;
    int cur_i = target_length;
    while (cur_length != target_length) {
        std::cout << arr_bricks[arr_with_length_options[cur_i] - 1].num << std::endl;
        cur_length += arr_bricks[arr_with_length_options[cur_i] - 1].length;
        cur_i = cur_i - arr_bricks[arr_with_length_options[cur_i] - 1].length;
    }    
} 

int* create_arr_with_length_options(Brick* arr_bricks, int n, int m) {
    int* arr_with_length_options = new int[m + 1];
    arr_with_length_options[0] = 0;
    for (int i = 1; i < m + 1; ++i) {
        arr_with_length_options[i] = -1;
    }
    for (int i = 0; i < n; ++i) {
        int brick_length = arr_bricks[i].length;
        for (int j = m - brick_length; j >= 0; --j) {
            if (arr_with_length_options[j] != -1) {
                if (arr_with_length_options[j + brick_length] == -1) {
                    arr_with_length_options[j + brick_length] = i + 1;
                }
            }
        }
    }
    return arr_with_length_options;
}

int check_arr_layers_lengths(int* arr_layers_lengths, int k) {
    for (int i = 1; i < k; ++i) {
        if (arr_layers_lengths[i] != arr_layers_lengths[i - 1]) {
            return 0;
        }
    }
    return arr_layers_lengths[0];
}

int main() {
    int n, k;
    std::cin >> n >> k;

    Brick** arr_bricks = new Brick*[k];
    for (int i = 0; i < k; ++i) {
        arr_bricks[i] = new Brick[50];
    }

    int* arr_layers_counts = new int[k];
    for (int i = 0; i < k; ++i) {
        arr_layers_counts[i] = 0;
    }

    int* arr_layers_lengths = new int[k];
    for (int i = 0; i < k; ++i) {
        arr_layers_lengths[i] = 0;
    }

    for (int i = 0; i < n; ++i) {
        int brick_length;
        int brick_color;
        std::cin >> brick_length;
        std::cin >> brick_color;
        arr_bricks[brick_color - 1][arr_layers_counts[brick_color - 1]].num = i + 1;
        arr_bricks[brick_color - 1][arr_layers_counts[brick_color - 1]].length = brick_length;
        ++(arr_layers_counts[brick_color - 1]);
        arr_layers_lengths[brick_color - 1] += brick_length;
    }

    int layer_length = check_arr_layers_lengths(arr_layers_lengths, k);
    if (!layer_length) {
        std::cout << "NO";
    } else {
        int** arrs_with_length_options = new int*[k];
        for (int i = 0; i < k; ++i) {
            arrs_with_length_options[i] = create_arr_with_length_options(arr_bricks[i], arr_layers_counts[i], layer_length);
        }
        int target_length = find_possible_one_wall_length(arrs_with_length_options, k, layer_length);
        if (!target_length) {
            std::cout << "NO";
        } else {
            std::cout << "YES" << std::endl;
            for (int i = 0; i < k; ++i) {
                restore_answer(arrs_with_length_options[i], arr_bricks[i], target_length);
            }

            for (int i = 0; i < k; ++i) {
                delete[] arrs_with_length_options[i];
            }
            delete[] arrs_with_length_options;
        }
    }

    for (int i = 0; i < k; ++i) {
        delete[] arr_bricks[i];
    }
    delete[] arr_bricks;

    delete[] arr_layers_counts;
    delete[] arr_layers_lengths;
    return 0;
}