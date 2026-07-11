#include "test.h"

#include "../arena.h"

#define VECTOR_IMPLEMENTATION
#include "../vector.h"
#include "../stack.h"
#include "../string.h"

Test(strings) {
    Arena *a = arena_init(NULL, 128);

    // match
    {
        arena_reset(a);
        Ok(str8_match(str8_lit("abc"),      str8_lit("abc")) == 0);
        Ok(str8_match(str8_lit("bbc"),      str8_lit("abc")) == 1);
        Ok(str8_match(str8_lit("abc"),      str8_lit("bbc")) == -1);
        Ok(str8_match(str8_lit("a"),        str8_lit("a")) == 0);
        // Ok(str8_match(str8_from_char('a'),  str8_lit("a")) == 0);
        Ok(str8_match(str8_lit("abcd"),     str8_lit("abc")) == 1);
        Ok(str8_match(str8_lit("abc"),      str8_lit("abcd")) == -1);
    }

    // reverse string
    {
        arena_reset(a);

        String8 str = *str8_alloc_cstr(a, "abcd");
        Ok(str8_match(str8_reverse(str), str8_lit("dcba")) == 0);
        Ok(str8_match(str8_reverse(str), str8_lit("abcd")) == 0);

        str = *str8_alloc_cstr(a, "abcde");
        Ok(str8_match(str8_reverse(str), str8_lit("edcba")) == 0);
        Ok(str8_match(str8_reverse(str), str8_lit("abcde")) == 0);
    }

    {
        arena_reset(a);

        Ok(u64_from_str8(str8_lit("1"),     Base10) == 1);
        Ok(u64_from_str8(str8_lit("21"),    Base10) == 21);
        Ok(u64_from_str8(str8_lit("285"),   Base10) == 285);
        Ok(u64_from_str8(str8_lit("3019"),  Base10) == 3019);
        Ok(u64_from_str8(str8_lit("30192"), Base10) == 30192);
    }
}

Test(arena) {
    Arena *a = arena_init(NULL, 64);

    // Cannot allocate more then capacity
    {
        arena_reset(a);

        Ok(arena_alloc(a, 32, 0) != NULL);
        Ok(arena_alloc(a, 32, 0) != NULL);
        Ok(arena_alloc(a, 32, 0) == NULL);
    }

    // Reset moves ptr to begining
    {
        arena_reset(a);

        void *ptr_1 = arena_alloc(a, 32, 0);
        arena_alloc(a, 32, 0);

        arena_reset(a);
        void *ptr_3 = arena_alloc(a, 64, 0);
        Ok(ptr_1 == ptr_3);
    }

    // Alloc should align data with paddings
    {
        arena_reset(a);
        arena_alloc(a, 3, 0);  // screw align

        void *ptr;
        ptr = arena_alloc(a, 4, 4);
        Ok(&a->data[4] == ptr);
        Ok(a->offset == 8);

        ptr = arena_alloc(a, 4, 4); // should be already aligned here
        Ok(&a->data[8] == ptr);
        Ok(a->offset == 12);
    }

    arena_free(a);
}

Test(vector) {
    Arena *a = arena_init(NULL, 1024);

    // vec_push and vec_last
    {
        u32 *v = vec_new(a, 4, sizeof(u32));

        vec_push(v, 1);
        Ok(vec_last(v) == 1);
        vec_push(v, 2);
        Ok(vec_last(v) == 2);
        vec_push(v, 3);
        Ok(vec_last(v) == 3);
        vec_push(v, 4);
        Ok(vec_last(v) == 4);
        vec_push(v, 5);
        Ok(vec_last(v) == 5);
        vec_push(v, 6);
        Ok(vec_last(v) == 6);

        Ok(vec_len(v) == 6);
        // Ok(vec_cap(v) == 8);
    }

    // vec_pop
    {
        u32 *v = vec_new(a, 4, sizeof(u32));

        vec_push(v, 10);
        vec_push(v, 20);
        Ok(vec_len(v) == 2);

        Ok(vec_pop(v) == 20);
        Ok(vec_pop(v) == 10);

        Ok(vec_len(v) == 0);
    }

    arena_free(a);
}

Test(stack) {
    Arena *a = arena_init(NULL, 128);

    {
        arena_reset(a);
        Stack *s = stack_new(a, 8, sizeof(u8));

        for (u8 i = 0; i < 5; i++) {   // {0, 1, 2, 3, 4}
            stack_push(s, &i);
        }
        Ok(s->length == 5);
        Ok(stack_is_empty(s) == false);

        Ok(cast(u8, stack_pop(s)) == 4);
        Ok(cast(u8, stack_pop(s)) == 3);
        Ok(cast(u8, stack_pop(s)) == 2);
        Ok(cast(u8, stack_pop(s)) == 1);
        Ok(cast(u8, stack_pop(s)) == 0);
        Ok(stack_is_empty(s) == true);

        Ok(stack_pop(s) == nullptr);
        Ok(stack_pop(s) == nullptr);
    }
}


int main() {
    RunTests();
    SummarizeTestsResult();

}
