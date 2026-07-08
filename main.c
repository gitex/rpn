#include <stdio.h>

// #define VECTOR_IMPLEMENTATION
// #include "vector.h"

#include "core.h"
#include "arena.h"
// #include "stack.h"
#include "string.h"
#include "tokenizer.h"


int main() {
    Arena *arena = arena_init(NULL, 1024);
    // Stack *stack = stack_new(arena, 32, sizeof(u32));
    // u8 *vec = vec_new(arena, 32, sizeof(u8));

    String8 str = str8_lit("2 2 + 1 2 + *");  // (2 + 2) * (1 + 2) = 4 * 3 = 12
    Token *token = next_token(arena, &str);
    str8_print(&token->str);
    token = next_token(arena, &str);
    str8_print(&token->str);
    token = next_token(arena, &str);
    str8_print(&token->str);

    arena_free(arena);

    return 0;
}
