// B. Ни больше ни меньше

// Дан массив целых положительных чисел a длины n. Разбейте его на минимально возможное количество отрезков,
// чтобы каждое число было не меньше длины отрезка, которому оно принадлежит. 
// Длиной отрезка считается количество чисел в нём.

// Разбиение массива на отрезки считается корректным, если каждый элемент принадлежит ровно одному отрезку.

// Формат ввода:
// Первая строка содержит одно целое число t (1 ≤ t ≤ 1 000) — количество наборов тестовых данных. 
// Затем следуют t наборов тестовых данных.

// Первая строка набора тестовых данных содержит одно целое число n (1 ≤ n ≤ 10^5) — длину массива.
// Следующая строка содержит n целых чисел a1, a2, ..., an (1 ≤ ai ≤ n) — массив a.

// Гарантируется, что сумма n по всем наборам тестовых данных не превосходит 2⋅10^5.

// Формат вывода:
// Для каждого набора тестовых данных в первой строке выведите число k — количество отрезков в вашем разбиении.
// Затем в следующей строке выведите k чисел len1, len2, ..., lenk (1 ≤ leni ≤ n, ∑ki=1leni=n) 
// — длины отрезков в порядке слева направо.


#include <iostream> 
#include <vector>

void part_on_sections(int* arr, int n) {
    int section_start = 0;
    int section_end = arr[0] - 1;
    int count_result = 0;
    std::vector<int> arr_result;
    for (int i = 1; i < n; ++i) {
        if (i <= section_end && arr[i] < section_end - section_start + 1) {
            section_end = section_start + arr[i] - 1;
        }
        if (i > section_end || arr[i] < (section_end - section_start + 1)) {
            ++count_result;
            arr_result.push_back(i - section_start);
            section_start = i;
            section_end = i + arr[i] - 1;
        }
    }
    ++count_result;
    arr_result.push_back(n - section_start);
    std::cout << count_result << std::endl;
    for (int i = 0; i < count_result; ++i) {
        std::cout << arr_result[i] << " ";
    }
    std::cout << std::endl;
}

int main() {
    int t;
    std::cin >> t;
    int** arrs_a = new int*[t];
    int* arr_k = new int[t];
    for (int i = 0; i < t; ++i) {
        int k;
        std::cin >> k;
        int* arr_a = new int[k];
        for (int j = 0; j < k; ++j) {
            std::cin >> arr_a[j];
        }
        arrs_a[i] = arr_a;
        arr_k[i] = k;
    }
    
    for (int i = 0; i < t; ++i) {
        part_on_sections(arrs_a[i], arr_k[i]);
    }

    for (int i = 0; i < t; ++ i) {
        delete[] arrs_a[i];
    }
    delete[] arrs_a;

    delete[] arr_k;
    return 0;
}