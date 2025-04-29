// !!!!!!!!!!!!!!!!!! медленно !!!!!!!!!!!!!!!!!!!!!!!!!!

// J. Упаковка и распаковка*

// Ваше решение будет запущено два раза.

// Вам необходимо реализовать алгоритм сжатия и распаковки, который будет сжимать тексты на английском языке, 
// записанные только маленькими английскими буквами, без пробелов, знаков препинания и вообще каких-либо символов, 
// отличных от маленьких английских букв. Эти тексты являются художественными произведениями (естественным текстом).

// Решение будет считаться верным, если в результате упаковки и распаковки получается исходная строка. 
// Также необходимо, чтобы количество байт в сжатой последовательности было как минимум вдвое меньше, 
// чем количество символов в исходной строке.

// Протокол взаимодействия:
// При первом запуске на вход программе в первой строке передается слово "pack".
// Во второй строке передается строка s из маленьких английских букв. 
// Длина строки для всех тестов находится в пределах от 100000 до 200000 букв.
// Ваше решение должно вывести количество чисел (байт) N в сжатой вами последовательности. 
// Во второй строке выведите N чисел от 0 до 255 включительно в десятичной системе счисления, разделяя их пробелами. 
// В конце обязательно сбросьте буфер вывода и выведите перевод строки.
// Количество байт не должно превышать половины от количества символов входной строки.
// При втором запуске на вход программе в первой строке передается слово "unpack".
// Затем в программу передаётся вывод первого запуска: количество байт, а затем числа от 0 до 255 через пробел 
// — последовательность символов. Необходимо вывести распакованную строку. 
// В конце обязательно сбросьте буфер вывода и выведите перевод строки.

// Примечания:
// Пусть ввод для первого запуска выглядит так:
// pack
// abacabaca
// Допустим, ваша программы вывела в качестве ответа:
// 5
// 11 255 0 13 253
// Добавляется слово unpack и этот ответ подается на вход вашей программе, ввод для второго запуска будет выглядеть так:
// unpack
// 5
// 11 255 0 13 253
// Ваша программа должна вывести
// abacabaca

#include <iostream>
#include <bitset>
#include <vector>
#include <queue>
#include <tuple>
#include <unordered_map>

typedef std::bitset<5> BitLetter;
typedef std::bitset<18> BitNumber;

class Letter {
    public:
        Letter(char letter_): bits(letter_ - 'a') {}
        Letter(BitLetter bits_): bits(bits_){};
        char ToChar() { return (char)(bits.to_ulong() + 'a'); };
        BitLetter bits;
        bool operator==(Letter& other) { return bits == other.bits; }
};

class Number {
    public:
        Number(): bits(0) {}
        Number(unsigned int number_): bits(number_) {};
        Number(BitNumber bits_): bits(bits_){};
        unsigned int ToUnsignedInt() { return (unsigned int)(bits.to_ulong()); };
        BitNumber bits;
        Number& operator++();
        bool operator==(Number& other) { return bits == other.bits; }
        bool operator<(Number& other);
};

class EncodedBitset {
    public:
        EncodedBitset();
        EncodedBitset(std::vector<unsigned char> encodedChars);
        void SetSize(Number number);
        void PushNumber(Number number);
        void PushLetter(Letter letter);
        void PushUnsignedChar(unsigned char c);
        Number ReadNumber();
        Letter ReadLetter();
        unsigned char ReadUnsignedChar();
        std::vector<unsigned char> ConvertToChars();
        std::bitset<100000 * 5 + 1000000 * 18 + 100> bits;
        int size;
        int readPos;
};

EncodedBitset::EncodedBitset() {
    readPos = 0;
    size = 0;
    Number emptyNumber;
    PushNumber(emptyNumber);
}

EncodedBitset::EncodedBitset(std::vector<unsigned char> encodedChars) {
    readPos = 0;
    size = 0;
    for (int i = 0; i < encodedChars.size(); ++i) {
        PushUnsignedChar(encodedChars[i]);
    }
}

void EncodedBitset::SetSize(Number number) {
    for (int i = 0; i < number.bits.size(); ++i) {
        bits[i] = number.bits[i];
    }
}

void EncodedBitset::PushNumber(Number number) {
    for (int i = 0; i < number.bits.size(); ++i) {
        bits[size++] = number.bits[i];
    }
}

void EncodedBitset::PushLetter(Letter letter) {
    for (int i = 0; i < letter.bits.size(); ++i) {
        bits[size++] = letter.bits[i];
    }
}

void EncodedBitset::PushUnsignedChar(unsigned char c) {
    for (int i = 0; i < (sizeof(unsigned char) * 8); ++i) {
        bits[size++] = c % 2;
        c >>= 1;
    }
}

Number EncodedBitset::ReadNumber() {
    BitNumber bitNumber;
    for (int i = readPos; i < readPos + bitNumber.size(); ++i) {
        bitNumber[i - readPos] = bits[i];
    }
    Number result(bitNumber);
    readPos += bitNumber.size();
    return result;
}

Letter EncodedBitset::ReadLetter() {
    BitLetter bitLetter;
    for (int i = readPos; i < readPos + bitLetter.size(); ++i) {
        bitLetter[i - readPos] = bits[i];
    }
    Letter result(bitLetter);
    readPos += bitLetter.size();
    return result;
}

unsigned char EncodedBitset::ReadUnsignedChar() {
    char c = 0;
    for (int i = readPos; i < readPos + (sizeof(unsigned char) * 8); ++i) {
        c += bits[i] * (1 << (i - readPos));
    }
    readPos += (sizeof(unsigned char) * 8);
    return c;
}

std::vector<unsigned char> EncodedBitset::ConvertToChars() {
    std::vector<unsigned char> result;
    while (size % (sizeof(unsigned char) * 8) != 0) {
        bits[size++] = 0;
    }
    for (int i = 0; i < (size / (sizeof(unsigned char) * 8)); ++i) {
        result.push_back(ReadUnsignedChar());
    }
    return result;
}

Number& Number::operator++() {
    int i = 0;
    while (bits[i] != 0) {
        bits[i++] = 0;
    }
    bits[i] = 1;
    return *this;
}

bool Number::operator<(Number& other) {
    for (int i = bits.size(); i >= 0; --i) {
        if (bits[i] ^ other.bits[i]) {
            return other.bits[i];
        }
    }
    return false;
}

class LZWTree {
    public:
        struct Node {
            Number number;
            Letter letter;
            std::vector<Node*> children;
        };
        LZWTree();
        ~LZWTree();
        std::tuple<Number, Node*, bool> InsertLetter(Letter letter, Node* startNode);
        std::pair<Node*, std::string> GetNodeByNumber(Node* startNode, Number number, std::string letters);
        EncodedBitset Encode(std::string text);
        std::string Decode(EncodedBitset encodedBitset);
        Node* root;
        Number size;
    private:
        void DestroyTree(Node* node); 
};

LZWTree::LZWTree() {
    Number number(0);
    Letter letter(' ');
    root = new Node{number, letter, {}};
}

LZWTree::~LZWTree() {
    DestroyTree(root);
}

std::tuple<Number, LZWTree::Node*, bool> LZWTree::InsertLetter(Letter letter, Node* startNode) {
    for (auto childElem: startNode->children) {
        if (childElem->letter == letter) {
            return std::make_tuple(childElem->number, childElem, false);
        }
    }
    ++size;
    Node* newNode = new Node({.number = size, .letter = letter, .children = {}});
    startNode->children.push_back(newNode);
    return std::make_tuple(startNode->number, newNode, true);
}

std::pair<LZWTree::Node*, std::string> LZWTree::GetNodeByNumber(Node* startNode, Number number, std::string letters) {
    Number emptyNumber;
    if (number == emptyNumber) {
        return std::make_pair(root, "");
    }
    if (startNode->number == number) {
        letters += startNode->letter.ToChar();
        return std::make_pair(startNode, letters);
    }
    for (auto childNode : startNode->children) {
        if ((childNode->number < number) || (childNode->number == number)) {
            std::pair<Node*, std::string> result = GetNodeByNumber(childNode, number, letters);
            Node* targetNode = result.first;
            if (targetNode != nullptr) {
                Number emptyNumber;
                if (!(startNode->number == emptyNumber)) {
                    letters = startNode->letter.ToChar() + result.second;
                } else {
                    letters = result.second;
                }
                return std::make_pair(targetNode, letters);
            }
        } else {
            break;
        }
    }
    return std::make_pair(nullptr, "");
}

void LZWTree::DestroyTree(Node* node) {
    for (auto childNode: node->children) {
        DestroyTree(childNode);
    }
    delete node;
}

EncodedBitset LZWTree::Encode(std::string text) {
    EncodedBitset encodedBitset;
    Number encodedBitsetSize;
    Node* curNode = root;
    Number prevNodeNumber;
    Node* encodedNode;
    bool isInserted = false;
    for (int i = 0; i < text.length(); ++i) {
        Letter letter(text[i]);
        auto result = InsertLetter(letter, curNode);
        prevNodeNumber = std::get<0>(result);
        encodedNode = std::get<1>(result);
        isInserted = std::get<2>(result);
        if (isInserted) {
            encodedBitset.PushNumber(prevNodeNumber);
            ++encodedBitsetSize;
            encodedBitset.PushLetter(letter);
            ++encodedBitsetSize;
            curNode = root;
        } else {
            curNode = encodedNode;
        }
    }
    if (isInserted == false) {
        encodedBitset.PushNumber(prevNodeNumber);
        ++encodedBitsetSize;
    }
    encodedBitset.SetSize(encodedBitsetSize);
    return encodedBitset;
}

std::string LZWTree::Decode(EncodedBitset encodedBitset) {
    std::string text = "";
    Number size = encodedBitset.ReadNumber();
    std::pair<Node*, std::string> result = std::make_pair(nullptr, "");
    for (int i = 0; i < size.ToUnsignedInt(); ++i) {
        if (i % 2 == 0) {
            Number number = encodedBitset.ReadNumber();
            result = GetNodeByNumber(root, number, "");
            text += result.second;
        } else {
            Letter letter = encodedBitset.ReadLetter();
            InsertLetter(letter, result.first);
            text += letter.ToChar();
        }
    }
    return text;
}

int main() {
    std::string command;
    std::cin >> command;
    if (command == "pack") {
        std::string text;
        std::cin >> text;
        LZWTree lzwTree;
        EncodedBitset encodedBitset = lzwTree.Encode(text);
        std::vector<unsigned char> encodedChars = encodedBitset.ConvertToChars();
        std::cout << encodedChars.size() << std::endl;
        for (auto c: encodedChars) {
            std::cout << static_cast<unsigned int>(c) << " ";
        }
        std::cout << std::endl;
    } else if (command == "unpack") {
        int n;
        std::cin >> n;
        std::vector<unsigned char> encodedChars(n);
        for (int i = 0; i < n; ++i) {
            unsigned int c;
            std::cin >> c;
            encodedChars[i] = c;
        }
        EncodedBitset encodedBitset(encodedChars);
        LZWTree lzwTree;
        std::cout << lzwTree.Decode(encodedBitset);
        std::cout << std::endl;
    }
    return 0;
}