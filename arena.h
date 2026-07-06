#include <stddef.h>
#include <inttypes.h>


typedef struct Arena {
    char *data;
    size_t capacity;
    size_t offset;
    struct Arena *next;
} Arena;


Arena *arena_init(Arena *a, size_t size);
void *arena_alloc(Arena *a, size_t size, size_t align);
void arena_reset(Arena *a);
void arena_free(Arena *a);
void arena_reset_to(Arena *a, void *ptr);


#define arena_alloc_type(a, T) (T*)arena_alloc(a, sizeof(T))
