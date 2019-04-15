#include "poolallocator.h"

#include <cassert>

static void testNullptr() {
    PoolAllocator<int, 10> pool;
    int *v = pool.alloc();
    assert(v);
}

static void testMultipleAllocations() {
    PoolAllocator<int, 10> pool;
    
    int *v0 = pool.alloc();
    int *v1 = pool.alloc();

    assert(v0 != v1);
}

static void testConstructor() {
    class C {
    public:
        C(int i) : _i(i) {}
        int i() const { return _i; }
    private:
        int _i;
    };

    PoolAllocator<C, 10> pool;
    C *v = pool.alloc(42);
    assert(v->i() == 42);
}

static void testDestructor() {
    class C {
    public:
        C(bool *p) : _destructorCalled(p) {
            *_destructorCalled = false;
        }
        ~C() { *_destructorCalled = true; }
    private:
        bool *_destructorCalled;
    };

    PoolAllocator<C, 10> pool;

    bool destructorCalled = false;    
    C *v = pool.alloc(&destructorCalled);
    pool.free(v);
    assert(destructorCalled);
}

int main()
{
    testNullptr();
    testMultipleAllocations();
    testConstructor();
    testDestructor();
}
