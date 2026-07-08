#ifndef STACK_H
#define STACK_H

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#include "core.h"
#include "arena.h"

#define STACK_MIN_CAPACITY 4

typedef struct {
    byte *items;
    size_t capacity;
    size_t length;
    size_t item_size;
    byte *next;
} Stack;

#define STACK_HEADER_SIZE (sizeof(Stack))

ptr stack_new(Arena *arena, size_t capacity, size_t item_size);
void stack_push(Stack *s, void *v);
byte *stack_pop(Stack *s);
byte *stack_peek(Stack *s);

#define stack_is_empty(s)    ((s)->length == 0)
#define stack_is_full(s)     ((s)->length == (s)->capacity)
// #define stack_pop_type(s, T) (cast(T, stack_pop((s))))

// #ifdef STACK_ALIAS
//     #define pop stack_pop
//     #define push stack_push
//     #define peek stack_peek
// #endif // !STACK_ALIAS


#endif // !STACK_H
