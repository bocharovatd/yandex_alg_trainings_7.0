// A. Количество единиц

// Подсчитайте количество единиц в двоичной записи числа.

// Формат ввода
// Дано целое число x (0 ≤ x ≤ 4^18), записанное в десятичной системе счисления.

// Формат вывода
// Вывести количество единиц в двоичной записи числа x.

#include <iostream>

int countOneInBinary(unsigned long long n) {
    int count = 0;
    for (int i = 0; i < 64; ++i) {
        count += (n & 1);
        n = n >> 1;
    }
    return count;
}

int main() {
    unsigned long long n;
    std::cin >> n;
    std::cout << countOneInBinary(n);
    return 0;
}