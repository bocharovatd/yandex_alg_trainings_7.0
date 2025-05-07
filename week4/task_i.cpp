// I. Снеговики

// Зима. 2012 год. На фоне грядущего Апокалипсиса и конца света незамеченной прошла новость об очередном прорыве 
// в областях клонирования и снеговиков: клонирования снеговиков. 
// Вы конечно знаете, но мы вам напомним, что снеговик состоит из нуля или более вертикально поставленных друг на друга шаров, 
// а клонирование — это процесс создания идентичной копии (клона). 

// В местечке Местячково учитель Андрей Сергеевич Учитель купил через интернет-магазин «Интернет-магазин аппаратов клонирования»
// аппарат для клонирования снеговиков. Теперь дети могут играть и даже играют во дворе в следующую игру: 
// время от времени один из них выбирает понравившегося снеговика, клонирует его и 
//     либо добавляет ему сверху один шар, 
//     либо удаляет из него верхний шар (если снеговик не пустой). 
// Учитель Андрей Сергеевич Учитель записал последовательность действий 
// и теперь хочет узнать суммарную массу всех построенных снеговиков.

// Формат ввода
// Первая строка содержит количество действий n (1 ≤ n ≤ 200 000). 
// В строке номер i + 1 содержится описание действия i: 
// t m — клонировать снеговика номер t (0 ≤ t < i) и добавить сверху шар массой m (0 < m ≤ 1000); 
// t 0 — клонировать снеговика номер t (0 ≤ t < i) и удалить верхний шар. 
// Гарантируется, что снеговик t не пустой. В результате действия i, описанного в строке i + 1 создается снеговик номер i. 
// Изначально имеется пустой снеговик с номером ноль. Все числа во входном файле целые.

// Формат вывода
// Выведите суммарную массу построенных снеговиков.

#include <iostream>
#include <vector>

class SumVersions {
public:
    SumVersions(size_t countVersions);
    void Push(size_t version, int value);
    void PushWithoutLastValue(size_t version);
    unsigned long Sum();
private:
    struct Node {
        size_t prevVersion;
        unsigned long sum;
    };
    std::vector<Node> sumVersions;
    unsigned long sum;
    size_t curVersion;
};

SumVersions::SumVersions(size_t countVersions) {
    sumVersions.resize(countVersions + 1);
    sumVersions[0].sum = 0;
    sum = 0;
    curVersion = 0;
}

void SumVersions::Push(size_t version, int value) {
    ++curVersion;
    sumVersions[curVersion].prevVersion = version;
    sumVersions[curVersion].sum = sumVersions[version].sum + value;
    sum += sumVersions[curVersion].sum;
}

void SumVersions::PushWithoutLastValue(size_t version) {
    ++curVersion;
    sumVersions[curVersion].prevVersion = sumVersions[sumVersions[version].prevVersion].prevVersion;
    sumVersions[curVersion].sum = sumVersions[sumVersions[version].prevVersion].sum;
    sum += sumVersions[curVersion].sum;
}

unsigned long SumVersions::Sum() {
    return sum;
}

int main() {
    int countVersions;
    std::cin >> countVersions;
    SumVersions sumVersions(countVersions);
    for (int i = 0; i < countVersions; ++i) {
        int version, value;
        std::cin >> version >> value;
        if (value != 0) {
            sumVersions.Push(version, value);
        } else {
            sumVersions.PushWithoutLastValue(version);
        }
    }
    std::cout << sumVersions.Sum();
    return 0;
}