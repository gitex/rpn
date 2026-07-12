#include "core.h"
#include "string.h"
#include "arena.h"
#include "tokenizer.h"
#include <assert.h>

#define VECTOR_IMPLEMENTATION
#include "vector.h"

// Rules:
//     - Every token divided by at least one space

Opnd op_add(Opnd a, Opnd b)             { return a + b; }
Opnd op_minus(Opnd a, Opnd b)           { return a - b; }
Opnd op_multiply(Opnd a, Opnd b)        { return a * b; }
Opnd op_divide(Opnd a, Opnd b)          { return a / b; }
Opnd op_modulo(Opnd a, Opnd b)          { return a % b; }
Opnd op_max2(Opnd a, Opnd b)            { return (a > b) ? a : b; }
Opnd op_min2(Opnd a, Opnd b)            { return (a < b) ? a : b; }
Opnd op_min3(Opnd a, Opnd b, Opnd c)     { return op_min2(op_min2(a, b), c); }
Opnd op_max3(Opnd a, Opnd b, Opnd c)     { return op_max2(op_max2(a, b), c); }

Token *next_token(Arena *arena, String8 *s) {
    *s = str8_trim(*s);

    byte *token_str_ptr = arena_alloc(arena, TOKEN_MAX_SIZE, TOKEN_MAX_SIZE);
    String8 token_str = str8(token_str_ptr, 0);

    byte *ch_ptr = s->chars;
    while(!char_is_space(*ch_ptr) && !char_is_nullterm(*ch_ptr)) {
        byte c = *ch_ptr;
        token_str = str8_concat(arena, token_str, str8_from_char(&c));
        ch_ptr += 1;
    }

    Token *token = arena_alloc(arena, sizeof(Token), ALIGN_8);

    // Is it number
    if (str8_is_u64(token_str, Base10)) {
        *token = (Token) {
            .str = token_str,
            .tag = TOK_UINT,
            .value = { .u = u64_from_str8(token_str, Base10)}
        };
    // ... or operation?
    } else {
        for (usize i = 0; i < ops_count; i++) {
            String8 op_word = *str8_from_cstr(arena, ops[i].word);
            if (str8_match(op_word, token_str) == 0) {
                *token = (Token) {
                    .str = token_str,
                    .tag = TOK_OPERATION,
                    .value = { .op = ops[i] },
                };
                break;
            }
        }

        // fprintf(stderr, "Cannot parse expression '%.*s'\n", token_str.len, token_str.chars);
        // exit(1);
    }

    *s = str8_skip(*s, token_str.len);
    return token;
}


Opnd calc_expression(String8 expr) {
    Arena *arena = arena_init(NULL, Kilobytes(32));

    Opnd *vec = vec_new(arena, 32, sizeof(u64));

    while(expr.len) {
        Token *token = next_token(arena, &expr);

        if (token->tag == TOK_UINT) {
            vec_push(vec, token->value.u);
        } else if (token->tag == TOK_OPERATION) {
            u64 result = 0;

            Op op = token->value.op;

            assert(op.arity <= vec_len(vec));

            Opnd a, b, c;
            switch (op.arity) {
                case 0:
                    result = op.fn.nullary();
                    break;
                case 1:
                    result = op.fn.unary(vec_pop(vec));
                    break;
                case 2:
                    // NOTE(Evgeniy): macros vec_pop contains length--
                    // Puting more then one vec_pop into func args may cause UB
                    a = vec_pop(vec);
                    b = vec_pop(vec);
                    result = op.fn.binary(a, b);
                    break;
                case 3:
                    // NOTE(Evgeniy): macros vec_pop contains length--
                    // Puting more then one vec_pop into func args may cause UB
                    a = vec_pop(vec);
                    b = vec_pop(vec);
                    c = vec_pop(vec);
                    result = op.fn.ternary(a, b, c);
                    break;
                default:
                    NotImplemented("Does not support arity more then 3");
            }

            vec_push(vec, result);
        }
    }

    Opnd result = vec_pop(vec);

    arena_free(arena);
    return result;
}


