#include "test.h"

#include "../arena.h"

Test(arena_allocations) {
    Arena *a = arena_init(NULL, 64);

    // Cannot allocate more then capacity
    {
        arena_reset(a);

        Assert(arena_alloc(a, 32, 0) != NULL);
        Assert(arena_alloc(a, 32, 0) != NULL);
        Assert(arena_alloc(a, 32, 0) == NULL);
    }

    // Reset moves ptr to begining
    {
        arena_reset(a);

        void *ptr_1 = arena_alloc(a, 32, 0);
        arena_alloc(a, 32, 0);

        arena_reset(a);
        void *ptr_3 = arena_alloc(a, 64, 0);
        Assert(ptr_1 == ptr_3);
    }

    // Alloc should align data with paddings
    {
        arena_reset(a);
        arena_alloc(a, 3, 0);  // screw align

        void *ptr;
        ptr = arena_alloc(a, 4, 4);
        Assert(&a->data[4] == ptr);
        Assert(a->offset == 8);

        ptr = arena_alloc(a, 4, 4); // should be already aligned here
        Assert(&a->data[8] == ptr);
        Assert(a->offset == 12);
    }

    arena_free(a);
}


int main() {
    RunTests();
    SummarizeTestsResult();

}
