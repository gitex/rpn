#include "core.h"
#include "string.h"
#include "arena.h"
#include "tokenizer.h"
#include <assert.h>

#define VECTOR_IMPLEMENTATION
#include "vector.h"


#define MAX_STACK_SIZE 64


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
    *s = str8_skip(*s, token_str.len);

    // Is it number
    if (str8_is_u64(token_str, Base10)) {
        *token = (Token) {
            .str = token_str,
            .tag = TOK_UINT,
            .value = { .u = u64_from_str8(token_str, Base10)}
        };
        return token;

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
                return token;
            }
        }
    }

    *token = (Token) {.str = token_str, .tag = TOK_UNKNOWN };
    return token;
}


Result calc_expression(Arena *arena, String8 expr) {
    if (expr.len == 0) {
        return (Result) {
            .err_msg = str8_f(arena, "Expression cannot be empty\n"),
        };
    }

    Opnd *vec = vec_new(arena, MAX_STACK_SIZE, sizeof(u64));

    while(expr.len) {
        Token *token = next_token(arena, &expr);

        if (token->tag == TOK_UNKNOWN) {
            return (Result) {
                .err_msg = str8_f(arena, "Invalid expression '%.*s'\n", token->str.len, token->str.chars),
            };
        }

        if (token->tag == TOK_UINT) {
            vec_push(vec, token->value.u);
        } else if (token->tag == TOK_OPERATION) {
            Op op = token->value.op;

            if (op.arity > vec_len(vec)) {
                return (Result) {
                    .err_msg = str8_f(arena, "Operation %s requires %u operands\n", op.word, op.arity),
                };
            }

            u64 result = 0;
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

    usize stack_len = vec_len(vec);
    if (stack_len > 1) {
        String8 opnds_left = str8_lit(""),
                sep = str8_lit(" "),
                last_opnd;

        while (!vec_is_empty(vec)) {
            last_opnd = str8_f(arena, "%d", vec_pop(vec));
            opnds_left = str8_concat_sep(arena, opnds_left, last_opnd, sep);
        }
        opnds_left = str8_reverse(str8_trim(opnds_left));
        return (Result) {
            .err_msg = str8_f(arena, "At the end %u operands left instead of 1 (%.*s), "
                    "maybe missing operator\n", stack_len, opnds_left.len, opnds_left.chars),
        };
    }

    return (Result) { .operand = vec_pop(vec) };
}


