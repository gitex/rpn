#include <stdio.h>
#include <assert.h>

#define VECTOR_IMPLEMENTATION
#include "vector.h"
#include "core.h"
#include "arena.h"
#include "string.h"
#include "tokenizer.h"

// (2 + 2) * (1 + 2) = 4 * 3 = 12
// 3 4 * 1 2 + / = 12 / 3 = 4


u64 parse_expression(String8 expr) {
    Arena *arena = arena_init(NULL, Kilobytes(32));

    u64 *vec = vec_new(arena, 32, sizeof(u64));

    while(expr.len) {
        Token *token = next_token(arena, &expr);

        if (token->tag == TOK_UINT) {
            vec_push(vec, token->value.u);
        } else if (token->tag == TOK_OPERATION) {
            u64 result = 0;

            Op op = token->value.op;

            assert(op.arity <= vec_len(vec));

            switch (op.arity) {
                case 0:
                    result = op.fn.nullary();
                    break;
                case 1:
                    result = op.fn.unary(vec_pop(vec));
                    break;
                case 2:
                    result = op.fn.binary(vec_pop(vec), vec_pop(vec));
                    break;
                case 3:
                    result = op.fn.ternary(vec_pop(vec), vec_pop(vec), vec_pop(vec));
                    break;
                default:
                    NotImplemented("Does not support arity more then 3");
            }

            vec_push(vec, result);
        }
    }

    u64 result = vec_pop(vec);
    arena_free(arena);

    return result;
}

int main() {
    Arena *arena = arena_init(NULL, Kilobytes(1));
    Arena *tmp = arena_init(NULL, Kilobytes(1));
    u64 *vec = vec_new(arena, 32, sizeof(u64));

    String8 str = str8_lit("3 4 * 1 2 3 max2 + /");
    u64 result = parse_expression(str);
    printf("%lu\n", result);
    return 0;
}
