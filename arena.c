#include <stddef.h>
#include <stdlib.h>

#include "types.h"
#include "arena.h"


Arena *arena_init(Arena *a, size_t size) {
    // if (a != NULL) {
    //     while (a->next) { a = a->next; }
    // }

    void *arena_ptr = malloc(size + sizeof(Arena));
    if (!arena_ptr) return NULL;

    Arena b = {
        .data = data_ptr(arena_ptr),
        .capacity = size,
        .offset = 0,
        .next = (a != NULL) ? a : NULL,
    };

    *(Arena *)arena_ptr = b;

    return arena_ptr;
}

static uptr calc_padding(uptr ptr, size_t align) {
    size_t mod = ptr % align;
    if (mod != 0) {
        return align - mod;
    }
    return 0;
}

void *arena_alloc(Arena *a, size_t size, size_t align) {
    if (size > a->capacity - a->offset) { return NULL; }

    if (align > 0) {
        uptr current = (uptr)a->data + a->offset;
        size_t padding = calc_padding(current, align);
        if (a->offset + padding + size > a->capacity) { return NULL; }

        a->offset += padding;
    }

    void *ptr = a->data + a->offset;
    a->offset += size;

    return ptr;
}
void arena_reset(Arena *a) {
    a->offset = 0;
}

void arena_free(Arena *a) {
    if (a->next != NULL) {
        arena_free(a->next);
    }
    free(a);
}

