#include <stdio.h>
#include "core.h"
#include "string.h"
#include "arena.h"
#include "tokenizer.h"

// char *str = "2 2 + 1 2 + *";  // (2 + 2) * (1 + 2) = 4 * 3 = 12
// Rules:
//     - Every token divided by at least one space

const u64 add(u64 a, u64 b)      { return a + b; }
const u64 minus(u64 a, u64 b)    { return a - b; }
const u64 multiply(u64 a, u64 b) { return a * b; }
const u64 divide(u64 a, u64 b)   { return a / b; }
const u64 max2(u64 a, u64 b)      { return (a > b) ? a : b; }
const u64 min2(u64 a, u64 b)      { return (a < b) ? a : b; }


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
        for (int i = 0; i < ops_count; i++) {
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


