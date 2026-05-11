#include <iostream>
#include <vector>
#include <map>
#include <memory>

// 1. Реализация аллокатора
template <typename T, size_t BlockSize = 10>
struct MyAllocator
{
    using value_type = T;

    struct State
    {
        std::vector<T *> chunks;
        size_t current_offset = 0;

        ~State()
        {
            for (auto ptr : chunks)
            {
                ::operator delete(ptr);
            }
        }
    };

    std::shared_ptr<State> state;

    MyAllocator() : state(std::make_shared<State>()) {}

    template <typename U>
    MyAllocator(const MyAllocator<U, BlockSize> &other) noexcept : state(other.state) {}

    T *allocate(size_t n)
    {
        if (n > BlockSize)
            throw std::bad_alloc();

        if (state->chunks.empty() || state->current_offset + n > BlockSize)
        {
            state->chunks.push_back(reinterpret_cast<T *>(::operator new(BlockSize * sizeof(T))));
            state->current_offset = 0;
        }

        T *result = state->chunks.back() + state->current_offset;
        state->current_offset += n;
        return result;
    }

    void deallocate(T *p, size_t n) noexcept
    {
        // По условию: "аллокатор освобождает всю память самостоятельно" в конце.
        // Пулы обычно не возвращают память по кускам, чтобы избежать фрагментации.
    }

    template <typename U, typename... Args>
    void construct(U *p, Args &&...args)
    {
        new (p) U(std::forward<Args>(args)...);
    }

    template <typename U>
    void destroy(U *p)
    {
        p->~U();
    }

    template <typename U>
    struct rebind
    {
        using other = MyAllocator<U, BlockSize>;
    };
};

// Операторы сравнения (обязательны для аллокаторов)
template <typename T, typename U, size_t S>
bool operator==(const MyAllocator<T, S> &a, const MyAllocator<U, S> &b) { return a.state == b.state; }
template <typename T, typename U, size_t S>
bool operator!=(const MyAllocator<T, S> &a, const MyAllocator<U, S> &b) { return !(a == b); }

// 2. Реализация своего контейнера

template <typename T, typename Allocator = std::allocator<T>>
class MyList
{
    struct Node
    {
        T value;
        Node *next;
        Node(const T &val) : value(val), next(nullptr) {}
    };

    using NodeAlloc = typename Allocator::template rebind<Node>::other;
    NodeAlloc alloc;
    Node *head = nullptr;
    Node *tail = nullptr;
    size_t _size = 0;

public:
    MyList() = default;
    ~MyList()
    {
        Node *current = head;
        while (current)
        {
            Node *next = current->next;
            alloc.destroy(current);
            alloc.deallocate(current, 1);
            current = next;
        }
    }

    void push_back(const T &value)
    {
        Node *newNode = alloc.allocate(1);
        alloc.construct(newNode, value);
        if (!head)
        {
            head = tail = newNode;
        }
        else
        {
            tail->next = newNode;
            tail = newNode;
        }
        _size++;
    }

    struct Iterator
    {
        Node *current;
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T *;
        using reference = T &;

        T &operator*() { return current->value; }
        Iterator &operator++()
        {
            current = current->next;
            return *this;
        }
        bool operator!=(const Iterator &other) const { return current != other.current; }
    };

    Iterator begin() { return Iterator{head}; }
    Iterator end() { return Iterator{nullptr}; }
    size_t size() const { return _size; }
    bool empty() const { return _size == 0; }
};

// 3. Прикладной код

long long factorial(int n)
{
    return (n == 0) ? 1 : n * factorial(n - 1);
}

int main()
{
    // 3.1 & 3.2: std::map
    std::map<int, int> m1;
    for (int i = 0; i < 10; ++i)
        m1[i] = (int)factorial(i);

    // 3.3 & 3.4: std::map + MyAllocator
    std::map<int, int, std::less<int>, MyAllocator<std::pair<const int, int>, 10>> m2;
    for (int i = 0; i < 10; ++i)
        m2[i] = (int)factorial(i);

    // 3.5: Вывод map
    for (auto const &pair : m2)
    {
        std::cout << pair.first << " " << pair.second << std::endl;
    }

    // 3.6 & 3.7: MyList + std::allocator
    MyList<int> l1;
    for (int i = 0; i < 10; ++i)
        l1.push_back(i);

    // 3.8 & 3.9: MyList + MyAllocator
    MyList<int, MyAllocator<int, 10>> l2;
    for (int i = 0; i < 10; ++i)
        l2.push_back(i);

    // 3.10: Вывод MyList
    for (auto val : l2)
    {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    return 0;
}