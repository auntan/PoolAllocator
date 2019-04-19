#pragma once

#include <utility>

// Dummy mutex declaration
struct PoolAllocatorEmptyMutex;

// Mutex should provide following methods:
// init(), lock(), unlock()
template <typename T, size_t POOL_SIZE, typename Mutex = PoolAllocatorEmptyMutex>
class PoolAllocator
{
public:    
    PoolAllocator() : 
        _head(_storage)
    {
        _mutex.init();

        _mutex.lock();
        for (size_t i = 1; i < POOL_SIZE; i++) {
            _storage[i - 1].setNext(&_storage[i]);
        }
        _storage[POOL_SIZE - 1].setNext(nullptr);
        _mutex.unlock();
    }

    template <typename... Args>
    T *alloc(Args &&... args)
    {
        _mutex.lock();
        Item *head = _head;
        _head = head->next();
        _mutex.unlock();

        T *result = head->placement();
        new (result) T(std::forward<Args>(args)...);

        return result;
    }
    
    void free(T *t)
    {
        t->T::~T();

        Item *item = reinterpret_cast<Item*>(t);
        item->setNext(_head);

        _mutex.lock();
        _head = item;
        _mutex.unlock();
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
        alignas(alignof(T)) char _data[sizeof(T)];
    };

private:
    Item _storage[POOL_SIZE];
    Item *_head;
    Mutex _mutex;
};

struct PoolAllocatorEmptyMutex
{
    void init() {}
    void lock() {}
    void unlock() {}
};