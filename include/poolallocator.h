#pragma once

#include <utility>

template <typename T, size_t POOL_SIZE>
class PoolAllocator
{
public:    
    PoolAllocator() : 
        _head(_storage)
    {
        for (size_t i = 1; i < POOL_SIZE; i++) {
            _storage[i - 1].setNext(&_storage[i]);
        }
        _storage[POOL_SIZE - 1].setNext(nullptr);
    }

    template <typename... Args>
    T *alloc(Args &&... args)
    {
        Item *head = _head;
        _head = head->next();

        T *result = head->placement();
        new (result) T(std::forward<Args>(args)...);

        return result;
    }
    
    void free(T *t)
    {
        t->T::~T();

        Item *item = reinterpret_cast<Item*>(t);
        item->setNext(_head);
        _head = item;
    }

private:
    union Item
    {
    public:
        Item *next() const { return _next; }
        void setNext(Item *n) { _next = n; }
        T *placement() { return reinterpret_cast<T*>(_data); }

    private:
        Item *_next;
        alignas(alignof(char[sizeof(T)])) char _data[sizeof(T)];
    };

private:
    Item _storage[POOL_SIZE];
    Item *_head;
};
