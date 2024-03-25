#pragma once

namespace ad
{
    template <typename T>
    class List
    {
    protected:
        struct Node
        {
            T value;
            Node* nextNode;
            Node* prevNode;

            Node(const T& _value) : value(_value), nextNode(nullptr), prevNode(nullptr) { }
        };
        Node* head;
        Node* tail;
        size_t size;

    public:
        List() : head(nullptr), tail(nullptr), size(0) { }
        List(const List<T>& other);
        List(List<T>&& other);
        ~List();

        class Iterator
        {
        private:
            Node* node;
        public:
            Iterator(Node* _node) : node(_node) { };
            T& operator*() { return node->value; };
            Iterator operator+(int right);
            Iterator& operator++() { node = node->nextNode; return *this; };
            Iterator& operator--() { node = node->prevNode; return *this; };
            bool operator!=(const Iterator& other) const { return node != other.node; };
            bool operator==(const Iterator& other) const { return node == other.node; }
        };

        Iterator begin() const { return Iterator(head); }
        Iterator end() const { return Iterator(nullptr); }

        void push_front(const T& value);
        void push_back(const T& value);
        void pop_front();
        void pop_back();
        T& front() const;
        T& back() const;
        bool empty() const;
        size_t getSize() const;
        void clear();
        void MergeSort(Node** headRef);

        T& operator[](int index);
        List<T>& operator=(const List<T>& other);
        List<T>& operator=(List<T>&& other);

    private:
        void FrontBackSplit(Node* source, Node** frontRef, Node** backRef);
        Node* SortedMerge(Node* a, Node* b);
    };
}

template<typename T>
void ad::List<T>::push_front(const T& value)
{
    Node* newNode = new Node(value);
    newNode->nextNode = head;
    if (tail == nullptr)
        tail = newNode;
    else
        head->prevNode = newNode;
    head = newNode;
    ++size;
}

template<typename T>
void ad::List<T>::push_back(const T& value)
{
    Node* newNode = new Node(value);
    if (tail == nullptr)
        head = newNode;
    else
    {
        tail->nextNode = newNode;
        newNode->prevNode = tail;
    }
    tail = newNode;
    ++size;
}

template<typename T>
void ad::List<T>::pop_front()
{
    Node* tmp = head;
    head = head->nextNode;
    head->prevNode = nullptr;
    delete tmp;
    if (head == nullptr)
        tail = nullptr;
    --size;
}

template<typename T>
void ad::List<T>::pop_back()
{
    Node* tmp = tail;
    tail = tail->prevNode;
    tail->nextNode = nullptr;
    delete tmp;
    if (tail == nullptr)
        head = nullptr;
    --size;
}

template<typename T>
T& ad::List<T>::front() const
{
    return head->value;
}

template<typename T>
T& ad::List<T>::back() const
{
    return tail->value;
}

template<typename T>
bool ad::List<T>::empty() const
{
    return size == 0;
}

template<typename T>
size_t ad::List<T>::getSize() const
{
    return size;
}

template<typename T>
void ad::List<T>::clear()
{
    while (head != nullptr)
        pop_back();
}

template<typename T>
inline void ad::List<T>::MergeSort(Node** headRef)
{
    Node* head = *headRef;
    Node* a;
    Node* b;

    if ((head == nullptr) || (head->nextNode == nullptr))
    {
        return;
    }

    FrontBackSplit(head, &a, &b);

    MergeSort(&a);
    MergeSort(&b);

    *headRef = SortedMerge(a, b);
}

template<typename T>
ad::List<T>::List(const List<T>& other)
{
    Node* temp = other.head;
    while (temp != nullptr)
    {
        push_back(temp->value);
        temp = temp->nextNode;
    }
}

template<typename T>
ad::List<T>::List(List<T>&& other) : head(other.head), tail(other.tail)
{
    other.head = nullptr;
    other.tail = nullptr;
}

template<typename T>
ad::List<T>::~List()
{
    while (head != nullptr)
    {
        Node* tmp = head;
        head = head->nextNode;
        delete tmp;
    }
    tail = nullptr;
    size = 0;
}

template<typename T>
T& ad::List<T>::operator[](int index)
{
    Node* tmp = head;
    for (int i = 0; i < index; ++i) { tmp = tmp->nextNode; }
    return tmp->value;
}

template<typename T>
ad::List<T>& ad::List<T>::operator=(const List<T>& other)
{
    if (this != &other)
    {
        clear();
        Node* temp = other.head;
        while (temp != nullptr)
        {
            push_back(temp->value);
            temp = temp->nextNode;
        }
    }
    return *this;
}

template<typename T>
ad::List<T>& ad::List<T>::operator=(List<T>&& other)
{
    if (this != &other)
    {
        clear();
        head = other.head;
        tail = other.tail;
        other.head = other.tail = nullptr;
    }
    return *this;
}

template<typename T>
void ad::List<T>::FrontBackSplit(Node* source, Node** frontRef, Node** backRef)
{
    Node* fast;
    Node* slow;
    slow = source;
    fast = source->nextNode;

    while (fast != nullptr)
    {
        fast = fast->nextNode;
        if (fast != nullptr)
        {
            slow = slow->nextNode;
            fast = fast->nextNode;
        }
    }
    *frontRef = source;
    *backRef = slow->nextNode;
    slow->nextNode = nullptr;
}

template<typename T>
ad::List<T>::Node* ad::List<T>::SortedMerge(Node* a, Node* b)
{
    Node* result = nullptr;

    if (a == nullptr)
        return b;
    else if (b == nullptr)
        return a;

    if (a->value <= b->value)
    {
        result = a;
        result->nextNode = SortedMerge(a->nextNode, b);
        if (result->nextNode != nullptr)
            result->nextNode->prevNode = result;
    }
    else
    {
        result = b;
        result->nextNode = SortedMerge(a, b->nextNode);
        if (result->nextNode != nullptr)
            result->nextNode->prevNode = result;
    }
    return result;
}

