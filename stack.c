#include <stddef.h>

#include "core.h"
#include "stack.h"
#include "arena.h"

#define byte_ptr(s, l) ((s)->items + (l) * (s)->item_size)

ptr stack_new(Arena *arena, size_t capacity, size_t item_size) {
    if (!arena) return nullptr;

    capacity = (capacity > STACK_MIN_CAPACITY) ? capacity : STACK_MIN_CAPACITY;
    capacity = next_pow2(capacity);

    byte *header_ptr = arena_alloc(arena, capacity * item_size + STACK_HEADER_SIZE, 0);
    if (!header_ptr) return nullptr;

    byte *items_ptr = header_ptr + STACK_HEADER_SIZE;
    *(Stack *)header_ptr = (Stack) {
        .items = items_ptr,
        .item_size = item_size,
        .capacity = capacity,
        .length = 0,
        .next = nullptr,
    };

    return header_ptr;
}

void stack_push(Stack *s, void *v) {
    if (!s || !v || s->length > s->capacity) return;
    *byte_ptr(s, s->length++) = cast(byte, v);
}

byte *stack_pop(Stack *s) {
    if (stack_is_empty(s)) return nullptr;
    return byte_ptr(s, --s->length);
}

byte *stack_peek(Stack *s) {
    return byte_ptr(s, s->length - 1);
}

