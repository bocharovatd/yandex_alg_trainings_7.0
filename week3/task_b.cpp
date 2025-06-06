// B. Миссия джедая Ивана

// Юный джедай Иван был заброшен на Звезду Смерти с заданием уничтожить её. 
// Для того, чтобы уничтожить Звезду Смерти, ему требуется массив неотрицательных целых чисел ai длины N. 
// К сожалению, у Ивана нет этого массива, но есть секретный документ с требованиями к этому массиву, 
// который ему передал его старый друг Дарт Вейдер.

// В этом документе содержится квадратная матрица m размера N, 
// где элемент в i-й строке в j-м столбце равен побитовому "И" чисел ai и aj. 
// Для повышения безопасности главная диагональ матрицы была уничтожена и вместо чисел на ней были записаны нули.
// Помогите Ивану восстановить массив a и выполнить свою миссию.

// Гарантируется, что решение всегда существует. Если решений несколько, выведите любое.

// Формат ввода
// В первой строке содержится число N (1 ≤ N ≤ 1000) — размер матрицы.

// Каждая из последующих N строк содержит по N целых чисел mij(0 ≤ mij​ ≤9) — элементы матрицы.

// Формат вывода
// В единственной строке выведите N целых неотрицательных чисел, не превышающих 100 — требуемый массив a.

#include <iostream>

int main() {
    unsigned short int n;
    std::cin >> n;
    unsigned short int* arr = new unsigned short int[n]();
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            unsigned short int value;
            std::cin >> value;
            arr[i] |= value;
        }
    }
    for (int i = 0; i < n; ++i) {
        std::cout << arr[i] << " ";
    }
    delete[] arr;
    return 0;
}