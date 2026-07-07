#ifndef VECTOR_H
#define VECTOR_H

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include "types.h"
#include "arena.h"


#define REALLOC(ctx, ptr, size)
#define FREE(ctx, ptr)


#if UINTPTR_MAX == 0xffffffffffffffff
    #define PLATFORM_64BIT 1
#elif UINTPTR_MAX == 0xffffffff
    #define PLATFORM_32BIT 1
#else
    #error "Unsupported pointer size"
#endif  // PLATFORM


#if PLATFORM_64BIT
    typedef i64* IVec;
#else
    typedef i32* IVec;
#endif  // IVec

#if PLATFORM_64BIT
    typedef u64* UVec;
#else
    typedef u32* UVec;
#endif  // IVec

#define VECTOR_IMPLEMENTATION
#ifdef VECTOR_IMPLEMENTATION

#define CAP_MIN 4
#define CAP_FACTOR 2

#define NEXT_CAP(curr_cap) ((curr_cap) * CAP_FACTOR)
#define PREV_CAP(curr_cap) ((curr_cap) / CAP_FACTOR > CAP_MIN ? (curr_cap) / CAP_FACTOR : CAP_MIN)

typedef struct {
    size_t capacity;
    size_t length;
    size_t elem_size;
    Arena *arena;
} VecHeader;

typedef void* VecPtr;


#define header_ptr(vec_ptr) ((VecHeader *) (vec_ptr) - 1)
#define vector_ptr(header_ptr) ((char *) (header_ptr) + sizeof(VecHeader))

#define vec_len(vec_ptr)    header_ptr((vec_ptr))->length
#define vec_cap(vec_ptr)    header_ptr((vec_ptr))->capacity
#define vec_push(vec, elem) (realloc_if_needed((vec)), (vec)[header_ptr((vec))->length++] = elem)
#define vec_pop(vec)        (header_ptr(vec)->length--, (vec)[header_ptr((vec)->length])
#define vec_last(vec)       ((vec)[header_ptr(vec)->length])

VecPtr vec_new(Arena *a, size_t capacity, size_t elem_size) {
    if (capacity < CAP_MIN) {
        capacity = CAP_MIN;
    }

    // void *ptr = arena_alloc(a, capacity * elem_size + sizeof(VecHeader), 32);
    void *ptr = malloc(capacity * elem_size + sizeof(VecHeader));
    if (!ptr) return NULL;

    void *vec_ptr = vector_ptr(ptr);

    VecHeader *h = header_ptr(vec_ptr);
    h->length = 0;
    h->capacity = capacity;
    h->elem_size = elem_size;

    return vec_ptr;
}

static void *realloc_if_needed(VecPtr vec) {
    VecHeader *h = header_ptr(vec);

    if (h->length == h->capacity) {
        size_t next_cap = NEXT_CAP(h->capacity);
        size_t new_size = next_cap * h->elem_size + sizeof(VecHeader);

        void *tmp = realloc(h, new_size);
        if (!tmp) return NULL;

        vec = vector_ptr(tmp);
        header_ptr(vec)->capacity = next_cap;
    }
    return vec;
}

#endif // VECTOR_IMPLEMENTATION

#endif  // VECTOR_H
