#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "core.h"
#include "arena.h"


Arena *arena_init(Arena *a, usize size) {
    void *arena_ptr = malloc(size + sizeof(Arena));
    if (!arena_ptr) return nullptr;

    Arena b = {
        .data = data_ptr(arena_ptr),
        .capacity = size,
        .offset = 0,
        .next = (a != NULL) ? a : nullptr,
    };

    *(Arena *)arena_ptr = b;

    return arena_ptr;
}

static uptr calc_padding(uptr ptr, usize align) {
    size_t mod = ptr % align;
    if (mod != 0) {
        return align - mod;
    }
    return 0;
}

void *arena_alloc(Arena *a, usize size, ArenaAlign align) {
    if (size > a->capacity - a->offset) { return NULL; }

    if (align != NO_ALIGN) {
        uptr current = (uptr)a->data + a->offset;
        size_t padding = calc_padding(current, align);
        if (a->offset + padding + size > a->capacity) {
            return NULL;
        }

        a->offset += padding;
    }

    void *ptr = a->data + a->offset;
    a->offset += size;
    // TODO: zero allocated memory? Or do different function for that?

    return ptr;
}

void *arena_alloc_zeroed(Arena *a, usize size, ArenaAlign align) {
    byte *start = arena_alloc(a, size, align);
    memset(start, 0, size);
    return start;
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

