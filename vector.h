#ifndef VECTOR_H
#define VECTOR_H

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "core.h"
#include "arena.h"


#define REALLOC(ctx, ptr, size)
#define FREE(ctx, ptr)

#ifdef VECTOR_IMPLEMENTATION

#define CAP_MIN 4
#define CAP_FACTOR 2
#define ALIGNING 32

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
#define calc_size(cap, elem_size) ((cap) * (elem_size) + sizeof(VecHeader))
#define curr_size(vec_ptr) calc_size(header_ptr((vec_ptr))->capacity, header_ptr((vec_ptr))->elem_size)


#define vec_len(vec_ptr)    header_ptr((vec_ptr))->length
#define vec_cap(vec_ptr)    header_ptr((vec_ptr))->capacity
#define vec_push(vec, elem) (realloc_if_needed((vec)), (vec)[header_ptr((vec))->length++] = elem)
#define vec_pop(vec)        (header_ptr((vec))->length--, ((vec))[header_ptr((vec))->length])
#define vec_last(vec)       (vec)[header_ptr(vec)->length - 1]

VecPtr vec_new(Arena *a, size_t cap, size_t elem_size) {
    if (cap < CAP_MIN) {
        cap = CAP_MIN;
    } else {
        cap = next_pow2(cap);
    }

    void *h_ptr;
    if (a != NULL) {
        h_ptr = arena_alloc(a, calc_size(cap, elem_size), ALIGNING);
    }
    if (!h_ptr) return NULL;

    void *vec_ptr = vector_ptr(h_ptr);

    VecHeader *h = header_ptr(vec_ptr);
    h->length = 0;
    h->capacity = cap;
    h->elem_size = elem_size;
    h->arena = a;

    return vec_ptr;
}

static VecPtr realloc_if_needed(VecPtr vec) {
    VecHeader *head = header_ptr(vec);

    if (head->length == head->capacity) {
        size_t next_cap = NEXT_CAP(head->capacity);
        size_t new_size = calc_size(next_cap, head->elem_size);

        void *new_header_ptr = NULL;
        if (head->arena != NULL) {
            new_header_ptr = arena_alloc(head->arena, new_size, ALIGNING);
        }
        if (new_header_ptr == NULL) return NULL;

        memcpy(new_header_ptr, head, calc_size(head->capacity, head->elem_size));

        vec = vector_ptr(new_header_ptr);
        header_ptr(vec)->capacity = next_cap;
    }
    return vec;
}

#endif // VECTOR_IMPLEMENTATION

#endif  // VECTOR_H
