#include <iostream>
#include <string>
#include <stdexcept>
#include <utility> // std::move

// ПОСЛЕДОВАТЕЛЬНЫЙ КОНТЕЙНЕР (динамический массив с резервированием)
template <typename T> //шаблон для создания конейнеров ращлыхных типов
class SerialContainer {
private:
    T* data;            // указатель на динамический массив элементов
    size_t current_size; // текущее количество элементов
    size_t capacity;     // выделенная вместимость

    // Внутренний метод: увеличивает capacity при необходимости
    // Использует move для эффективного переноса элементов
    void reserve(size_t new_capacity) {
        if (new_capacity <= capacity) return;
        T* newData = new T[new_capacity];
        for (size_t i = 0; i < current_size; ++i) 
            newData[i] = std::move(data[i]);  // перемещаем, а не копируем
        delete[] data;
        data = newData;
        capacity = new_capacity;
    }

public:
    // ИТЕРАТОР (прямой доступ)
    class Iterator {
        T* ptr;
    public:
        Iterator(T* p) : ptr(p) {}
        T& operator*() { return *ptr; }
        Iterator& operator++() { ptr++; return *this; }
        bool operator!=(const Iterator& other) const { return ptr != other.ptr; }
    };
    Iterator begin() { return Iterator(data); }
    Iterator end() { return Iterator(data + current_size); }

    // Базовые конструкторы и деструктор для создания контейнеров
    SerialContainer() : data(nullptr), current_size(0), capacity(0) {}
    
    // MOVE-КОНСТРУКТОР и MOVE-ПРИСВАИВАНИЕ (Задание 4)
    // Перехватываем владение ресурсами, оставляя другой объект пустым
    SerialContainer(SerialContainer&& other) noexcept 
        : data(other.data), current_size(other.current_size), capacity(other.capacity) {
        other.data = nullptr; 
        other.current_size = 0; 
        other.capacity = 0;
    }
    SerialContainer& operator=(SerialContainer&& other) noexcept {
        if (this != &other) {
            delete[] data;                    // освобождаем старые ресурсы
            data = other.data; 
            current_size = other.current_size; 
            capacity = other.capacity;
            other.data = nullptr; 
            other.current_size = 0; 
            other.capacity = 0;
        }
        return *this;
    }
    //деструктор
    ~SerialContainer() { delete[] data; }

    // Методы доступа
    void push_back(const T& value) {   // l-value версия (копирование)
        if (current_size == capacity) 
            reserve((capacity == 0) ? 1 : static_cast<size_t>(capacity * 1.5 + 1));
        data[current_size++] = value;
    }
    
    void push_back(T&& value) {        // r-value версия (перемещение)
        if (current_size == capacity) 
            reserve((capacity == 0) ? 1 : static_cast<size_t>(capacity * 1.5 + 1));
        data[current_size++] = std::move(value);
    }
    
    // Вставка по индексу со сдвигом вправо
    void insert(size_t index, const T& value) {
        if (index > current_size) throw std::out_of_range("Index error");
        if (current_size == capacity) 
            reserve((capacity == 0) ? 1 : static_cast<size_t>(capacity * 1.5 + 1));
        // Сдвигаем элементы вправо, начиная с конца
        for (size_t i = current_size; i > index; --i) 
            data[i] = std::move(data[i - 1]);
        data[index] = value;
        current_size++;
    }
    
    // Удаление по индексу со сдвигом влево
    void erase(size_t index) {
        if (index >= current_size) throw std::out_of_range("Index error");
        for (size_t i = index; i < current_size - 1; ++i) 
            data[i] = std::move(data[i + 1]);
        current_size--;
    }
    
    size_t size() const { return current_size; }
    T& operator[](size_t index) { return data[index]; }
    
    // Вспомогательный метод для вывода (только для демонстрации)
    void print() const {
        for (size_t i = 0; i < current_size; ++i) 
            std::cout << data[i] << (i == current_size - 1 ? "" : ", ");
        std::cout << std::endl;
    }
};


// ОДНОНАПРАВЛЕННЫЙ СВЯЗНЫЙ СПИСОК
template <typename T> //шаблон для создания конейнеров ращлыхных типов
class SingleListContainer {
private:
    struct Node {
        T value; 
        Node* next;
        Node(T val) : value(std::move(val)), next(nullptr) {}
    };
    Node* head;         // указатель на первый элемент
    size_t current_size;
    
public:
    // Итератор для последовательного обхода
    class Iterator {
        Node* curr;
    public:
        Iterator(Node* n) : curr(n) {}
        T& operator*() { return curr->value; }
        Iterator& operator++() { if (curr) curr = curr->next; return *this; }
        bool operator!=(const Iterator& other) const { return curr != other.curr; }
    };
    Iterator begin() { return Iterator(head); }
    Iterator end() { return Iterator(nullptr); }

    SingleListContainer() : head(nullptr), current_size(0) {}
    
    // Move-конструктор: забираем голову, обнуляем источник
    SingleListContainer(SingleListContainer&& other) noexcept 
        : head(other.head), current_size(other.current_size) {
        other.head = nullptr; 
        other.current_size = 0;
    }
    
    // Деструктор: последовательно удаляем все узлы
    ~SingleListContainer() { 
        while (head) { 
            Node* t = head; 
            head = head->next; 
            delete t; 
        } 
    }

    // Добавление в конец (O(n) из-за обхода до хвоста)
    void push_back(const T& value) {
        Node* newNode = new Node(value);
        if (!head) head = newNode;
        else { 
            Node* temp = head; 
            while (temp->next) temp = temp->next; 
            temp->next = newNode; 
        }
        current_size++;
    }
    
    // Вставка по индексу: особая обработка для начала списка
    void insert(size_t index, const T& value) {
        if (index > current_size) throw std::out_of_range("Index error");
        Node* newNode = new Node(value);
        if (index == 0) { 
            newNode->next = head; 
            head = newNode; 
        } else {
            Node* prev = head;
            for (size_t i = 0; i < index - 1; ++i) prev = prev->next;
            newNode->next = prev->next; 
            prev->next = newNode;
        }
        current_size++;
    }
    
    // Удаление по индексу: также нужен доступ к предыдущему узлу
    void erase(size_t index) {
        if (index >= current_size) throw std::out_of_range("Index error");
        Node* toDelete;
        if (index == 0) { 
            toDelete = head; 
            head = head->next; 
        } else {
            Node* prev = head;
            for (size_t i = 0; i < index - 1; ++i) prev = prev->next;
            toDelete = prev->next; 
            prev->next = toDelete->next;
        }
        delete toDelete;
        current_size--;
    }
    
    size_t size() const { return current_size; }
    
    // Доступ по индексу (O(n) — неэффективно, но реализовано для единообразия)
    T& operator[](size_t index) {
        Node* curr = head;
        for (size_t i = 0; i < index; ++i) curr = curr->next;
        return curr->value;
    }
    
    void print() const {
        Node* curr = head;
        while (curr) { 
            std::cout << curr->value << (curr->next ? ", " : ""); 
            curr = curr->next; 
        }
        std::cout << std::endl;
    }
};

// ЗАДАНИЕ 3, 4, 5
// ДВУНАПРАВЛЕННЫЙ СВЯЗНЫЙ СПИСОК (doubly linked list)
// Хранит указатели и на next, и на prev + хвост для быстрого push_back

template <typename T> //шаблон для создания конейнеров ращлыхных типов
class DoubleListContainer {
private:
    struct Node {
        T value; 
        Node *next, *prev;
        Node(T val) : value(std::move(val)), next(nullptr), prev(nullptr) {}
    };
    Node *head, *tail;   // указатели на оба конца
    size_t current_size;
    
public:
    class Iterator {
        Node* curr;
    public:
        Iterator(Node* n) : curr(n) {}
        T& operator*() { return curr->value; }
        Iterator& operator++() { if (curr) curr = curr->next; return *this; }
        bool operator!=(const Iterator& other) const { return curr != other.curr; }
    };
    Iterator begin() { return Iterator(head); }
    Iterator end() { return Iterator(nullptr); }

    DoubleListContainer() : head(nullptr), tail(nullptr), current_size(0) {}
    
    // Move-семантика: забираем оба конца
    DoubleListContainer(DoubleListContainer&& other) noexcept 
        : head(other.head), tail(other.tail), current_size(other.current_size) {
        other.head = other.tail = nullptr; 
        other.current_size = 0;
    }
    
    ~DoubleListContainer() { 
        while (head) { 
            Node* t = head; 
            head = head->next; 
            delete t; 
        } 
    }

    // Добавление в конец за O(1) благодаря указателю tail
    void push_back(const T& value) {
        Node* newNode = new Node(value);
        if (!tail) head = tail = newNode;
        else { 
            tail->next = newNode; 
            newNode->prev = tail; 
            tail = newNode; 
        }
        current_size++;
    }
    
    // Вставка: частные случаи — начало и конец списка
    void insert(size_t index, const T& value) {
        if (index > current_size) throw std::out_of_range("Index error");
        if (index == current_size) { push_back(value); return; }
        
        Node* newNode = new Node(value);
        if (index == 0) { 
            newNode->next = head; 
            head->prev = newNode; 
            head = newNode; 
        } else {
            Node* curr = head;
            for (size_t i = 0; i < index; ++i) curr = curr->next;
            newNode->prev = curr->prev; 
            newNode->next = curr;
            curr->prev->next = newNode; 
            curr->prev = newNode;
        }
        current_size++;
    }
    
    // Удаление с корректным переподвязыванием соседей
    void erase(size_t index) {
        if (index >= current_size) throw std::out_of_range("Index error");
        Node* toDelete = head;
        for (size_t i = 0; i < index; ++i) toDelete = toDelete->next;
        
        if (toDelete->prev) toDelete->prev->next = toDelete->next; 
        else head = toDelete->next;
        
        if (toDelete->next) toDelete->next->prev = toDelete->prev; 
        else tail = toDelete->prev;
        
        delete toDelete;
        current_size--;
    }
    
    size_t size() const { return current_size; }
    
    T& operator[](size_t index) {
        Node* curr = head;
        for (size_t i = 0; i < index; ++i) curr = curr->next;
        return curr->value;
    }
    
    void print() const {
        Node* curr = head;
        while (curr) { 
            std::cout << curr->value << (curr->next ? ", " : ""); 
            curr = curr->next; 
        }
        std::cout << std::endl;
    }
};

// ДЕМОНСТРАЦИОННАЯ ФУНКЦИЯ
// Тестирует все три контейнера на одинаковом наборе операций:
// 1. Заполнение 0..9
// 2. Удаление элементов с индексами 2,3,4
// 3. Вставка в начало, середину и конец
// 4. Обход через итератор

template <typename Container> //шаблон для создания конейнеров ращлыхных типов
void run_demo(const std::string& label) {
    std::cout << "=== " << label << " ===" << std::endl;
    Container c;                              // 1.1: создание пустого контейнера

    for (int i = 0; i < 10; ++i) c.push_back(i); // 1.2: заполнение
    std::cout << "1.3 Содержимое: "; c.print();   // 1.3: вывод
    std::cout << "1.4 Размер: " << c.size() << std::endl; // 1.4: размер

    // 1.5: удаление элементов 
    c.erase(2); c.erase(3); c.erase(4); 
    std::cout << "1.6 После удаления третьего, пятого и седьмого элементов: "; c.print();

    c.insert(0, 10);                           // 1.7: вставка в начало
    std::cout << "1.8 После доб. 10 в начало: "; c.print();

    c.insert(c.size() / 2, 20);                // 1.9: вставка в середину
    std::cout << "1.10 После доб. 20 в середину: "; c.print();

    c.push_back(30);                           // 1.11: добавление в конец
    std::cout << "1.12 После доб. 30 в конец: "; c.print();

    std::cout << "Обход через итератор: ";
    for (auto it = c.begin(); it != c.end(); ++it) 
        std::cout << *it << " ";
    std::cout << "\n\n";
}

//точка входа
int main() {
    try {
        //вызов функции с аргументами шаблона и параметром 
        run_demo<SerialContainer<int>>("ПОСЛЕДОВАТЕЛЬНЫЙ"); //версия функции с типом <SerialContainer<int>>
        run_demo<SingleListContainer<int>>("ОДНОНАПРАВЛЕННЫЙ");
        run_demo<DoubleListContainer<int>>("ДВУНАПРАВЛЕННЫЙ");
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }
    return 0;
}