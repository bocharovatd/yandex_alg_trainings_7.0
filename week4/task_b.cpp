// B. Очередь с защитой от ошибок

// Научитесь пользоваться стандартной структурой данных queue для целых чисел. 
// Напишите программу, содержащую описание очереди и моделирующую работу очереди, реализовав все указанные здесь методы. 

// Программа считывает последовательность команд и в зависимости от команды выполняет ту или иную операцию. 
// После выполнения каждой команды программа должна вывести одну строчку.

// Возможные команды для программы:

// push n
// Добавить в очередь число n (значение n задается после команды). Программа должна вывести ok.

// pop
// Удалить из очереди первый элемент. Программа должна вывести его значение.

// front
// Программа должна вывести значение первого элемента, не удаляя его из очереди.

// size
// Программа должна вывести количество элементов в очереди.

// clear
// Программа должна очистить очередь и вывести ok.

// exit
// Программа должна вывести bye и завершить работу.

// Перед исполнением операций front и pop программа должна проверять, содержится ли в очереди хотя бы один элемент. Если во входных данных встречается операция front или pop, и при этом очередь пуста, то программа должна вместо числового значения вывести строку error.

// Формат ввода
// Вводятся команды управления очередью, по одной на строке

// Формат вывода
// Требуется вывести протокол работы очереди, по одному сообщению на строке

#include <iostream>
#include <string>

class Error {
public:
    Error(bool is_error_) : is_error(is_error_) {};
    bool operator!() {return !is_error; }
private:
    bool is_error;
};

class Queue {
public:
    Queue() : head(nullptr), tail(nullptr), size(0) {};

    void Push(int value) {
        Node* newNode = new Node;
        newNode->next = nullptr;
        newNode->value = value;
        if (size == 0) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
        ++size;
    }

    Error Pop(int& value) {
        if (this->IsEmpty()) {
            return Error(true);
        }
        Node* deleteNode = head;
        head = head->next;
        value = deleteNode->value;
        delete deleteNode;
        --size;
        return Error(false);
    }

    Error Front(int& value) {
        if (this->IsEmpty()) {
            return Error(true);
        }
        value = head->value;
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
        Node* next;
    };
    Node* tail;
    Node* head;
    size_t size;
};

int main() {
    std::string command;
    Queue queue;
    std::cin >> command;
    while (command != "exit") {
        if (command == "push") {
            int value;
            std::cin >> value;
            queue.Push(value);
            std::cout << "ok" << std::endl;
        } else if (command == "pop") {
            int value;
            Error err = queue.Pop(value);
            if (!err) {
                std::cout << value << std::endl;
            } else {
                std::cout << "error" << std::endl;
            }
        } else if (command == "front") {
            int value;
            Error err = queue.Front(value);
            if (!err) {
                std::cout << value << std::endl;
            } else {
                std::cout << "error" << std::endl;
            }
        } else if (command == "size") {
            std::cout << queue.Size() << std::endl;
        } else if (command == "clear") {
            queue.Clear();
            std::cout << "ok" << std::endl;
        } 
        std::cin >> command;
    }
    queue.Clear();
    std::cout << "bye" << std::endl;
    return 0;
}
