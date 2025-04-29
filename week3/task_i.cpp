// I. Исправление одной ошибки*

// Это задача с двойным запуском.

// Ваше решение будет запущено два раза.

// При первом запуске ему на вход будет передана строка x из нулей и единиц длиной не больше n. 
// Программа должна вывести строку y из нулей и единиц, длина которой не более чем m. 
// Число m не известно вашей программе и зависит от подзадачи, 
// соответствующее значение указано в таблице системы оценивания. 
// Если ваша программа выведет строку длиннее, чем m, она получит вердикт «Wrong answer».

// Между запусками решения программа жюри внесёт в строку y не более одной модификации, 
// заменив ноль на единицу или единицу на ноль, получив, таким образом, строку z.

// При втором запуске программе на вход будет подана строка z. 
// Она должна восстановить исходную строку x и выдать её на выход.

// Формат ввода
// При первом запуске на первой строке ввода находится число 1. 
// На второй строке ввода находится строка x из нулей и единиц длины n (10≤n≤100000).

// При втором запуске на первой строке ввода находится число 2. 
// На второй строке ввода находится строка z из нулей и единиц длины не больше m (10≤m≤100017). 
// Гарантируется, что эта строка равна строке y, выведенной программой при первом запуске, 
// или получена из неё изменением ровно одного символа на противоположный.

// Формат вывода
// При первом запуске необходимо вывести строку y, которая позволит восстановить x после внесения в неё изменения. 
// Длина строки y не должна превышать m, m не должно превосходить n более чем на log2n округленный вверх.
// При втором запуске по заданной строке z необходимо восстановить исходную строку x и вывести её.

// Примечания
// Пусть ввод для первого запуска выглядит так:
// 1
// 01010
// Допустим, ваша программы вывела в качестве ответа:
// 000111000111000
// В этот ответ вносится одно изменение и подается на вход вашей программе, например,
// ввод для второго запуска может выглядеть так:
// 2
// 000101000111000
// Ваша программа должна вывести
// 01010

#include <iostream>
#include <bitset>
#include <string>
#include <math.h>

bool isPowOfTwo(int x) {
    return x && !(x & (x - 1));
}

class HammingCode {
    public:
        HammingCode(std::bitset<100017> hammingCodeBits_, int size_) : hammingCodeBits(hammingCodeBits_), size(size_) {};
        HammingCode(std::bitset<100000> bits_, int bitsSize_);
        void FixError();
        void PrintDecoded();
        void Print();
    private:
        std::bitset<100017> hammingCodeBits;
        int size;
        int smForIPow2(int onePos);
};

int HammingCode::smForIPow2(int onePos) {
    int sm = 0;
    for (int enumI = 1; enumI < (1 << static_cast<int>(std::ceil(log2(size)) - 1)); ++enumI) {
        int curI;
        curI = ((enumI >> onePos) << 1) + 1;
        curI = (curI << onePos) + enumI % (1 << onePos);
        if (curI > size) {
            break;
        }
        sm += hammingCodeBits[curI - 1];
    }
    return sm;
}

HammingCode::HammingCode(std::bitset<100000> bits_, int bitsSize_) {
    int iHammingCodeBits = 0;
    for (int iBits = 0; iBits < bitsSize_; ++iBits) {
        if (!isPowOfTwo(iHammingCodeBits + 1)) {
            hammingCodeBits[iHammingCodeBits] = bits_[iBits];
        } else {
            --iBits;
        }
        ++iHammingCodeBits;
    }
    size = iHammingCodeBits;
    for (int onePos = 0; (1 << onePos) <= size; ++onePos) {
        int iPow2Bits = (1 << onePos);
        int sm = smForIPow2(onePos);
        hammingCodeBits[iPow2Bits - 1] = sm % 2;
    }
}

void HammingCode::FixError() {
    int iError = 0;
    for (int onePos = 0; (1 << onePos) <= size; ++onePos) {
        int iPow2Bits = (1 << onePos);
        int sm = smForIPow2(onePos);
        if (sm % 2 != hammingCodeBits[iPow2Bits - 1]) {
            iError |= iPow2Bits;
        }
    }
    if (iError) {
        hammingCodeBits[iError - 1] = ~hammingCodeBits[iError - 1];
    }
}

void HammingCode::PrintDecoded() {
    std::string result = "";
    for (int i = 0; i < size; ++i) {
        if (!isPowOfTwo(i + 1)) {
            result += (hammingCodeBits[i] ? "1" : "0");
        }
    }
    std::cout << result << std::endl;
}

void HammingCode::Print() {
    std::string result = "";
    for (int i = 0; i < size; ++i) {
        result += (hammingCodeBits[i] ? "1" : "0");
    }
    std::cout << result << std::endl;
}

int main() {
    int type;
    std::cin >> type;
    std::string bitsString;
    std::cin >> bitsString;
    if (type == 1) {
        std::bitset<100000> bits;
        for (int i = 0; i < bitsString.length(); ++i) {
            bits[i] = (bitsString[i] == '1'? 1 : 0);
        }
        HammingCode hammingCode(bits, bitsString.length());
        hammingCode.Print();
    } else if (type == 2) {
        std::bitset<100017> hammingCodeBits;
        for (int i = 0; i < bitsString.length(); ++i) {
            hammingCodeBits[i] = (bitsString[i] == '1' ? 1 : 0);
        }
        HammingCode hammingCode(hammingCodeBits, bitsString.length());
        hammingCode.FixError();
        hammingCode.PrintDecoded();
    }
    return 0;
}