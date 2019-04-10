No dependencies static memory pool allocator, suitable for embedded systems

## Usage

```cpp
#include "poolallocator.h"

struct Test
{
    Test(int x) { printf("Test(%d)\n", x); }
    ~Test() { printf("~Test()\n"); };
};

int main()
{
    // Declare objects type, and pool size
    PoolAllocator<Test, 10> pool;
    
    // Allocate memory from pool and 
    // forward arguments to Test constructor
    Test *test = pool.alloc(42);
    
    // Free memory and call destructor
    pool.free(test);
    
    return 0;
}
```
Output:
```
Test(42)
~Test()
```
