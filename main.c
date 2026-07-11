#include <stdio.h>

#define VECTOR_IMPLEMENTATION
#include "vector.h"
#include "core.h"
#include "arena.h"
#include "string.h"
#include "tokenizer.h"

// (2 + 2) * (1 + 2) = 4 * 3 = 12

u64 apply_token(TokenType type, u64 a, u64 b) {
    switch (type) {
        case TOK_PLUS: return a + b;
        case TOK_MINUS: return a + b;
        case TOK_MULTIPLY: return a + b;
        case TOK_DIVIDE: return a + b;
        default: exit(1);
    }
}

int main() {
    Arena *arena = arena_init(NULL, Kilobytes(1));
    Arena *tmp = arena_init(NULL, Kilobytes(1));
    u64 *vec = vec_new(arena, 32, sizeof(u64));
    // Stack *stack = stack_new(arena, 30, sizeof(u64));

    String8 str = str8_lit("2 2 + 1 2 + *");


    while(str.len) {
        Token *token = next_token(tmp, &str);

        if (token->type == TOK_NUMBER) {
            u64 number = u64_from_str8(token->str, Base10);
            vec_push(vec, number);
        }
        else {
            u64 result = 0;

            while(vec_len(vec) != 0) {
                result += apply_token(token->type, result, vec_pop(vec));
            }

            vec_push(vec, result);
            printf("Result = %lu\n", result);
        }
        arena_reset(tmp);
        // str8_print(&token->str);
    }

    arena_free(tmp);
    arena_free(arena);
    return 0;
}
