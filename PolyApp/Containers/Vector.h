#pragma once
#include <malloc.h>

namespace ad
{
    template<typename T>
    class Vector
    {
    private:
        T* elems_;
        size_t size_;
        size_t capacity_;

        static void construct(T* object);
        static void destroy(T* object);
        static T* allocate(int count);
        static void deallocate(void* ptr);
    public:
        Vector();
        Vector(const Vector<T>& other);
        Vector(Vector<T>&& other);
        Vector(const T _array[]);
        Vector(const T* const _array, const size_t _size);
        Vector(const size_t _size);
        Vector(const size_t _size, const T&);
        ~Vector();

        class Iterator
        {
        private:
            T* ptr_;
        public:
            Iterator(T* _ptr) : ptr_(_ptr) { };
            T& operator*() { return *ptr_; };
            Iterator operator+(int right) { return Iterator(this->ptr_ + right); };
            Iterator& operator++() { ++ptr_; return *this; };
            bool operator!=(const Iterator& other) const { return ptr_ != other.ptr_; };
        };

        Iterator begin();
        const Iterator cbegin() const;
        Iterator end();
        const Iterator cend() const;

        void reserve(size_t newCapacity);
        size_t size() const;
        size_t capacity() const;
        void push_back(const T& elem);
        void pop_back();
        T& front();
        const T& front() const;
        T& back();
        const T& back() const;
        bool empty() const;
        void clear();
        void resize(size_t _newSize);
        void resize(size_t _newSize, const T& value);
        void shrink_to_fit();
        void insert(Iterator position, const T& elem);

        T& operator[](size_t index);
        const T& operator[](size_t index) const;
        Vector<T>& operator=(const Vector<T>& other);
        Vector<T>& operator=(Vector<T>&& other) noexcept;
    };
}

template<typename T>
ad::Vector<T>::Vector()
{
    size_ = 0;
    capacity_ = 1;
    elems_ = allocate(capacity_);
}

template<typename T>
ad::Vector<T>::Vector(const T* const _array, const size_t _size)
{
    elems_ = allocate(_size);
    for (size_t i = 0; i < _size; i++)
    {
        construct(&elems_[i]);
        elems_[i] = _array[i];
    }
    size_ = capacity_ = _size;
}

template<typename T>
ad::Vector<T>::Vector(const size_t _size)
{
    elems_ = allocate(_size);
    size_ = 0;
    capacity_ = _size;
}

template<typename T>
ad::Vector<T>::Vector(const size_t _size, const T& value)
{
    elems_ = allocate(_size);
    for (auto elem : *this)
    {
        construct(&elem);
        elem = value;
    }
    size_ = capacity_ = _size;
}

template<typename T>
ad::Vector<T>::Vector(const Vector<T>& other)
{
    size_ = other.size_;
    capacity_ = other.capacity_;
    elems_ = allocate(other.size_);
    for (size_t i = 0; i < size_; i++)
    {
        construct(&elems_[i]);
        elems_[i] = other.elems_[i];
    }
}

template<typename T>
ad::Vector<T>::Vector(Vector<T>&& other) {
    elems_ = other.elems_;
    size_ = other.size_;
    capacity_ = other.capacity_;
    other.elems_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}

template<typename T>
ad::Vector<T>::Vector(const T _array[])
{
    size_ = sizeof(_array) / sizeof(_array[0]);
    elems_ = allocate(size_);
    capacity_ = size_;
    for (size_t i = 0; i < size_; i++)
    {
        construct(&elems_[i]);
        elems_[i] = _array[i];
    }

}

template<typename T>
ad::Vector<T>::~Vector()
{
    for (T& object : *this)
        destroy(&object);
    deallocate(elems_);
}

template<typename T>
ad::Vector<T>::Iterator ad::Vector<T>::begin()
{
    return &elems_[0];
}

template<typename T>
const typename ad::Vector<T>::Iterator ad::Vector<T>::cbegin() const
{
    return &elems_[0];
}

template<typename T>
ad::Vector<T>::Iterator ad::Vector<T>::end()
{
    return &elems_[size_];
}

template<typename T>
const typename ad::Vector<T>::Iterator ad::Vector<T>::cend() const
{
    return &elems_[size_];
}

template<typename T>
void ad::Vector<T>::reserve(size_t newCapacity)
{
    if (capacity_ < newCapacity)
    {
        T* newElems(allocate(newCapacity));
        for (size_t i = 0; i < capacity_; i++)
        {
            construct(&newElems[i]);
            newElems[i] = elems_[i];
        }
        if (capacity_ != 0)
        {
            for (auto object : *this)
                destroy(&object);
            deallocate(elems_);
        }
        elems_ = newElems;
        capacity_ = newCapacity;
    }
}

template<typename T>
size_t ad::Vector<T>::size() const
{
    return size_;
}

template<typename T>
size_t ad::Vector<T>::capacity() const
{
    return capacity_;
}

template<typename T>
void ad::Vector<T>::push_back(const T& elem)
{
    if (size_ == capacity_)
    {
        reserve(capacity_ == 0 ? 1 : capacity_ + capacity_ / 2 + 1);
    }
    construct(&elems_[size_]);
    elems_[size_++] = elem;
}

template<typename T>
void ad::Vector<T>::pop_back()
{
    destroy(&back());
    --size_;
}

template<typename T>
T& ad::Vector<T>::front()
{
    return elems_[0];
}

template<typename T>
const T& ad::Vector<T>::front() const
{
    return elems_[0];
}

template<typename T>
T& ad::Vector<T>::back()
{
    return elems_[size_ - 1];
}

template<typename T>
const T& ad::Vector<T>::back() const
{
    return elems_[size_ - 1];
}

template<typename T>
bool ad::Vector<T>::empty() const
{
    return size_ == 0;
}

template<typename T>
void ad::Vector<T>::clear()
{
    if (size_ > 0) {
        for (auto object : *this)
            destroy(&object);
        size_ = 0;
    }
}

template<typename T>
void ad::Vector<T>::resize(size_t _newSize)
{
    if (size_ > _newSize)
    {
        for (auto i = _newSize; i < size_; i++)
        {
            destroy(&elems_[i]);
        }
        size_ = _newSize;
    }
    else
        if (size_ < _newSize)
        {
            reserve(_newSize);
            for (auto i = size_; i < _newSize; i++)
            {
                construct(&elems_[i]);
            }
            size_ = _newSize;
        }
}

template<typename T>
void ad::Vector<T>::resize(size_t _newSize, const T& value)
{
    if (size_ > _newSize)
    {
        for (auto i = _newSize; i < size_; i++)
        {
            destroy(&elems_[i]);
        }
        size_ = _newSize;
    }
    else
        if (size_ < _newSize)
        {
            reserve(_newSize);
            for (auto i = size_; i < _newSize; i++)
            {
                construct(&elems_[i]);
                elems_[i] = value;
            }
            size_ = _newSize;
        }
}

template<typename T>
void ad::Vector<T>::shrink_to_fit()
{
    if (size_ < capacity_) {
        T* newElements(allocate(size_));
        for (size_t i = 0; i < size_; i++) {
            construct(&newElements[i]);
            newElements[i] = elems_[i];
            //new(&newElements[i]) T(std::move(elems_[i]));
            destroy(&elems_[i]);
        }
        deallocate(elems_);
        elems_ = newElements;
        capacity_ = size_;
    }
}

template<typename T>
void ad::Vector<T>::insert(Iterator position, const T& elem)
{
    *position = elem;
}

template<typename T>
T& ad::Vector<T>::operator[](size_t index)
{
    return elems_[index];
}

template<typename T>
const T& ad::Vector<T>::operator[](size_t index) const
{
    return elems_[index];
}

template<typename T>
ad::Vector<T>& ad::Vector<T>::operator=(const Vector<T>& other)
{
    if (this != &other)
    {
        for (auto object : *this)
            destroy(&object);
        deallocate(elems_);
        elems_ = allocate(other.capacity_);
        size_ = other.size_;
        capacity_ = other.capacity_;
        for (size_t i = 0; i < size_; i++) {
            construct(&elems_[i]);
            elems_[i] = other.elems_[i];
        }
    }
    return *this;
}

template<typename T>
ad::Vector<T>& ad::Vector<T>::operator=(Vector<T>&& other) noexcept
{
    if (this != &other)
    {
        delete[] elems_;
        elems_ = other.elems_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        other.elems_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }
    return *this;
}

template<typename T>
void ad::Vector<T>::construct(T* object)
{
    new(object) T;
}

template<typename T>
void ad::Vector<T>::destroy(T* object)
{
    object->~T();
}

template<typename T>
T* ad::Vector<T>::allocate(int count)
{
    return static_cast<T*>(malloc(count * sizeof(T)));
}

template<typename T>
void ad::Vector<T>::deallocate(void* ptr)
{
    free(ptr);
}


