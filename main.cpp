#include <iostream>
#include <string>
#include <stdexcept>
#include <utility> // std::move

//ЗАДАНИЕ 1, 2, 4, 5: ПОСЛЕДОВАТЕЛЬНЫЙ КОНТЕЙНЕР (С РЕЗЕРВОМ И MOVE)
template <typename T>
class SerialContainer {
private:
    T* data;
    size_t current_size;
    size_t capacity;

    void reserve(size_t new_capacity) {
        if (new_capacity <= capacity) return;
        T* newData = new T[new_capacity];
        for (size_t i = 0; i < current_size; ++i) newData[i] = std::move(data[i]);
        delete[] data;
        data = newData;
        capacity = new_capacity;
    }

public:
    // ИТЕРАТОР
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

    SerialContainer() : data(nullptr), current_size(0), capacity(0) {}
    
    // MOVE-СЕМАНТИКА
    SerialContainer(SerialContainer&& other) noexcept : data(other.data), current_size(other.current_size), capacity(other.capacity) {
        other.data = nullptr; other.current_size = 0; other.capacity = 0;
    }
    SerialContainer& operator=(SerialContainer&& other) noexcept {
        if (this != &other) {
            delete[] data; data = other.data; current_size = other.current_size; capacity = other.capacity;
            other.data = nullptr; other.current_size = 0; other.capacity = 0;
        }
        return *this;
    }
    ~SerialContainer() { delete[] data; }

    void push_back(const T& value) { // l-value
        if (current_size == capacity) reserve((capacity == 0) ? 1 : static_cast<size_t>(capacity * 1.5 + 1));
        data[current_size++] = value;
    }
    void push_back(T&& value) { // r-value (Задание 4)
        if (current_size == capacity) reserve((capacity == 0) ? 1 : static_cast<size_t>(capacity * 1.5 + 1));
        data[current_size++] = std::move(value);
    }
    void insert(size_t index, const T& value) {
        if (index > current_size) throw std::out_of_range("Index error");
        if (current_size == capacity) reserve((capacity == 0) ? 1 : static_cast<size_t>(capacity * 1.5 + 1));
        for (size_t i = current_size; i > index; --i) data[i] = std::move(data[i - 1]);
        data[index] = value;
        current_size++;
    }
    void erase(size_t index) {
        if (index >= current_size) throw std::out_of_range("Index error");
        for (size_t i = index; i < current_size - 1; ++i) data[i] = std::move(data[i + 1]);
        current_size--;
    }
    size_t size() const { return current_size; }
    T& operator[](size_t index) { return data[index]; }
    void print() const {
        for (size_t i = 0; i < current_size; ++i) std::cout << data[i] << (i == current_size - 1 ? "" : ", ");
        std::cout << std::endl;
    }
};

//ЗАДАНИЕ 3, 4, 5: ОДНОНАПРАВЛЕННЫЙ СПИСОК
template <typename T>
class SingleListContainer {
private:
    struct Node {
        T value; Node* next;
        Node(T val) : value(std::move(val)), next(nullptr) {}
    };
    Node* head; size_t current_size;
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

    SingleListContainer() : head(nullptr), current_size(0) {}
    SingleListContainer(SingleListContainer&& other) noexcept : head(other.head), current_size(other.current_size) {
        other.head = nullptr; other.current_size = 0;
    }
    ~SingleListContainer() { while (head) { Node* t = head; head = head->next; delete t; } }

    void push_back(const T& value) {
        Node* newNode = new Node(value);
        if (!head) head = newNode;
        else { Node* temp = head; while (temp->next) temp = temp->next; temp->next = newNode; }
        current_size++;
    }
    void insert(size_t index, const T& value) {
        if (index > current_size) throw std::out_of_range("Index error");
        Node* newNode = new Node(value);
        if (index == 0) { newNode->next = head; head = newNode; }
        else {
            Node* prev = head;
            for (size_t i = 0; i < index - 1; ++i) prev = prev->next;
            newNode->next = prev->next; prev->next = newNode;
        }
        current_size++;
    }
    void erase(size_t index) {
        if (index >= current_size) throw std::out_of_range("Index error");
        Node* toDelete;
        if (index == 0) { toDelete = head; head = head->next; }
        else {
            Node* prev = head;
            for (size_t i = 0; i < index - 1; ++i) prev = prev->next;
            toDelete = prev->next; prev->next = toDelete->next;
        }
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
        while (curr) { std::cout << curr->value << (curr->next ? ", " : ""); curr = curr->next; }
        std::cout << std::endl;
    }
};

//ЗАДАНИЕ 3, 4, 5: ДВУНАПРАВЛЕННЫЙ СПИСОК
template <typename T>
class DoubleListContainer {
private:
    struct Node {
        T value; Node *next, *prev;
        Node(T val) : value(std::move(val)), next(nullptr), prev(nullptr) {}
    };
    Node *head, *tail; size_t current_size;
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
    DoubleListContainer(DoubleListContainer&& other) noexcept : head(other.head), tail(other.tail), current_size(other.current_size) {
        other.head = other.tail = nullptr; other.current_size = 0;
    }
    ~DoubleListContainer() { while (head) { Node* t = head; head = head->next; delete t; } }

    void push_back(const T& value) {
        Node* newNode = new Node(value);
        if (!tail) head = tail = newNode;
        else { tail->next = newNode; newNode->prev = tail; tail = newNode; }
        current_size++;
    }
    void insert(size_t index, const T& value) {
        if (index > current_size) throw std::out_of_range("Index error");
        if (index == current_size) { push_back(value); return; }
        Node* newNode = new Node(value);
        if (index == 0) { newNode->next = head; head->prev = newNode; head = newNode; }
        else {
            Node* curr = head;
            for (size_t i = 0; i < index; ++i) curr = curr->next;
            newNode->prev = curr->prev; newNode->next = curr;
            curr->prev->next = newNode; curr->prev = newNode;
        }
        current_size++;
    }
    void erase(size_t index) {
        if (index >= current_size) throw std::out_of_range("Index error");
        Node* toDelete = head;
        for (size_t i = 0; i < index; ++i) toDelete = toDelete->next;
        if (toDelete->prev) toDelete->prev->next = toDelete->next; else head = toDelete->next;
        if (toDelete->next) toDelete->next->prev = toDelete->prev; else tail = toDelete->prev;
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
        while (curr) { std::cout << curr->value << (curr->next ? ", " : ""); curr = curr->next; }
        std::cout << std::endl;
    }
};

//ДЕМОНСТРАЦИЯ 
template <typename Container>
void run_demo(const std::string& label) {
    std::cout << "=== " << label << " ===" << std::endl;
    Container c; // 1.1

    for (int i = 0; i < 10; ++i) c.push_back(i); // 1.2
    std::cout << "1.3 Содержимое: "; c.print(); // 1.3
    std::cout << "1.4 Размер: " << c.size() << std::endl; // 1.4

    c.erase(2); c.erase(3); c.erase(4); // 1.5 (удаление 3, 5, 7)
    std::cout << "1.6 После удаления: "; c.print();

    c.insert(0, 10); // 1.7
    std::cout << "1.8 После доб. 10 в начало: "; c.print();

    c.insert(c.size() / 2, 20); // 1.9
    std::cout << "1.10 После доб. 20 в середину: "; c.print();

    c.push_back(30); // 1.11
    std::cout << "1.12 После доб. 30 в конец: "; c.print();

    std::cout << "Обход через итератор: ";
    for (auto it = c.begin(); it != c.end(); ++it) std::cout << *it << " ";
    std::cout << "\n\n";
}

int main() {
    try {
        run_demo<SerialContainer<int>>("ПОСЛЕДОВАТЕЛЬНЫЙ");
        run_demo<SingleListContainer<int>>("ОДНОНАПРАВЛЕННЫЙ");
        run_demo<DoubleListContainer<int>>("ДВУНАПРАВЛЕННЫЙ");
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }
    return 0;
}