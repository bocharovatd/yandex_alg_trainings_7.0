// C. Дек с защитой от ошибок

// Научитесь пользоваться стандартной структурой данных deque для целых чисел.  
// Напишите программу, содержащую описание дека и моделирующую работу дека, реализовав все указанные здесь методы. 
// Программа считывает последовательность команд и в зависимости от команды выполняет ту или иную операцию. 
// После выполнения каждой команды программа должна вывести одну строчку.

// Возможные команды для программы:

// push_front n
// Добавить (положить) в начало дека новый элемент. Программа должна вывести ok.

// push_back n
// Добавить (положить) в конец дека новый элемент. Программа должна вывести ok.

// pop_front
// Извлечь из дека первый элемент. Программа должна вывести его значение.

// pop_back
// Извлечь из дека последний элемент. Программа должна вывести его значение.

// front
// Узнать значение первого элемента (не удаляя его). Программа должна вывести его значение.

// back
// Узнать значение последнего элемента (не удаляя его). Программа должна вывести его значение.

// size
// Вывести количество элементов в деке.

// clear
// Очистить дек (удалить из него все элементы) и вывести ok.

// exit
// Программа должна вывести bye и завершить работу.

// Гарантируется, что количество элементов в деке в любой момент не превосходит 100. 
// Перед исполнением операций pop_front, pop_back, front, back программа должна проверять, 
// содержится ли в деке хотя бы один элемент. 
// Если во входных данных встречается операция pop_front, pop_back, front, back, и при этом дек пуст, 
// то программа должна вместо числового значения вывести строку error.

// Формат ввода
// Вводятся команды управления деком, по одной на строке.

// Формат вывода
// Требуется вывести протокол работы дека, по одному сообщению на строке

#include <iostream>
#include <string>

class Error {
public:
    Error(bool is_error_) : is_error(is_error_) {};
    bool operator!() {return !is_error; }
private:
    bool is_error;
};

class Dequeue {
public:
    Dequeue() : head(nullptr), tail(nullptr), size(0) {};

    void PushFront(int value) {
        Node* newNode = new Node;
        newNode->next = head;
        newNode->prev = nullptr;
        newNode->value = value;
        if (size == 0) {
            head = newNode;
            tail = newNode;
        } else {
            head->prev = newNode;
            head = newNode;
        }
        ++size;
    }

    void PushBack(int value) {
        Node* newNode = new Node;
        newNode->next = nullptr;
        newNode->prev = tail;
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

    Error PopFront(int& value) {
        if (this->IsEmpty()) {
            return Error(true);
        }
        Node* deleteNode = head;
        head = head->next;
        if (head != nullptr) {
            head->prev = nullptr;
        }
        value = deleteNode->value;
        delete deleteNode;
        --size;
        return Error(false);
    }

    Error PopBack(int& value) {
        if (this->IsEmpty()) {
            return Error(true);
        }
        Node* deleteNode = tail;
        tail = tail->prev;
        if (tail != nullptr) {
            tail->next = nullptr;
        }
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
            this->PopBack(value);
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
        Node* next;
    };
    Node* tail;
    Node* head;
    size_t size;
};

int main() {
    std::string command;
    Dequeue dequeue;
    std::cin >> command;
    while (command != "exit") {
        if (command == "push_front") {
            int value;
            std::cin >> value;
            dequeue.PushFront(value);
            std::cout << "ok" << std::endl;
        } else if (command == "push_back") {
            int value;
            std::cin >> value;
            dequeue.PushBack(value);
            std::cout << "ok" << std::endl;
        }
        else if (command == "pop_front") {
            int value;
            Error err = dequeue.PopFront(value);
            if (!err) {
                std::cout << value << std::endl;
            } else {
                std::cout << "error" << std::endl;
            }
        } else if (command == "pop_back") {
            int value;
            Error err = dequeue.PopBack(value);
            if (!err) {
                std::cout << value << std::endl;
            } else {
                std::cout << "error" << std::endl;
            }
        } else if (command == "front") {
            int value;
            Error err = dequeue.Front(value);
            if (!err) {
                std::cout << value << std::endl;
            } else {
                std::cout << "error" << std::endl;
            }
        } else if (command == "back") {
            int value;
            Error err = dequeue.Back(value);
            if (!err) {
                std::cout << value << std::endl;
            } else {
                std::cout << "error" << std::endl;
            }
        } else if (command == "size") {
            std::cout << dequeue.Size() << std::endl;
        } else if (command == "clear") {
            dequeue.Clear();
            std::cout << "ok" << std::endl;
        } 
        std::cin >> command;
    }
    dequeue.Clear();
    std::cout << "bye" << std::endl;
    return 0;
}
