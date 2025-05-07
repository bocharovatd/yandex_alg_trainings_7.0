// A. Стек с защитой от ошибок

// Научитесь пользоваться стандартной структурой данных stack для целых чисел. 
// Напишите программу, содержащую описание стека и моделирующую работу стека, реализовав все указанные здесь методы. 
// Программа считывает последовательность команд и в зависимости от команды выполняет ту или иную операцию. 
// После выполнения каждой команды программа должна вывести одну строчку. Возможные команды для программы:

// push n
// Добавить в стек число n (значение n задается после команды). Программа должна вывести ok.

// pop
// Удалить из стека последний элемент. Программа должна вывести его значение.

// back
// Программа должна вывести значение последнего элемента, не удаляя его из стека.

// size
// Программа должна вывести количество элементов в стеке.

// clear
// Программа должна очистить стек и вывести ok.

// exit
// Программа должна вывести bye и завершить работу.

// Перед исполнением операций back и pop программа должна проверять, содержится ли в стеке хотя бы один элемент. Если во входных данных встречается операция back или pop, и при этом стек пуст, то программа должна вместо числового значения вывести строку error.

// Формат ввода
// Вводятся команды управления стеком, по одной на строке

// Формат вывода
// Программа должна вывести протокол работы стека, по одному сообщению на строке

#include <iostream>
#include <string>

class Error {
public:
    Error(bool is_error_) : is_error(is_error_) {};
    bool operator!() {return !is_error; }
private:
    bool is_error;
};

class Stack {
public:
    Stack() : tail(nullptr), size(0) {};

    void Push(int value) {
        Node* newNode = new Node;
        newNode->prev = tail;
        newNode->value = value;
        tail = newNode;
        ++size;
    }

    Error Pop(int& value) {
        if (this->IsEmpty()) {
            return Error(true);
        }
        Node* deleteNode = tail;
        tail = tail->prev;
        value = deleteNode->value;
        delete deleteNode;
        --size;
        return Error(false);
    }

    Error Back(int& value) {
        if (this->IsEmpty()) {
            return Error(true);
        }
        value = tail->value;
        return Error(false);
    }

    size_t Size() {
        return size;
    }

    void Clear() {
        int value;
        while (!this->IsEmpty()) {
            this->Pop(value);
        }
    }

    bool IsEmpty() {
        if (size == 0) {
            return true;
        }
        return false;
    }

private:
    struct Node {
        int value;
        Node* prev;
    };
    Node* tail;
    size_t size;
};

int main() {
    std::string command;
    Stack stack;
    std::cin >> command;
    while (command != "exit") {
        if (command == "push") {
            int value;
            std::cin >> value;
            stack.Push(value);
            std::cout << "ok" << std::endl;
        } else if (command == "pop") {
            int value;
            Error err = stack.Pop(value);
            if (!err) {
                std::cout << value << std::endl;
            } else {
                std::cout << "error" << std::endl;
            }
        } else if (command == "back") {
            int value;
            Error err = stack.Back(value);
            if (!err) {
                std::cout << value << std::endl;
            } else {
                std::cout << "error" << std::endl;
            }
        } else if (command == "size") {
            std::cout << stack.Size() << std::endl;
        } else if (command == "clear") {
            stack.Clear();
            std::cout << "ok" << std::endl;
        } 
        std::cin >> command;
    }
    stack.Clear();
    std::cout << "bye" << std::endl;
    return 0;
}
