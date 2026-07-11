#ifndef ARENA_H
#define ARENA_H

#include "core.h"


typedef struct Arena {
    char *data;
    size_t capacity;
    size_t offset;
    struct Arena *next;
} Arena;

typedef enum ArenaAlign {
    NO_ALIGN = 0,
    ALIGN_2  = 1 << 1,
    ALIGN_4  = 1 << 2,
    ALIGN_8  = 1 << 3,
    ALIGN_16 = 1 << 4,
    ALIGN_32 = 1 << 5,
    ALIGN_64 = 1 << 6,
} ArenaAlign;

#define arena_ptr(data_ptr) ((Arena *) (data_ptr) - 1)
#define data_ptr(arena_ptr) ((char *) (arena_ptr) + sizeof(Arena))

Arena *arena_init(Arena *a, usize size);
void *arena_alloc(Arena *a, usize size, ArenaAlign align);
void arena_reset(Arena *a);
void arena_free(Arena *a);
void arena_reset_to(Arena *a, void *ptr);

#define arena_alloc_type(a, T) (T*)arena_alloc(a, sizeof(T))

#endif  // ARENA_H
