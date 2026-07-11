#include "string.h"
#include "arena.h"
#include "tokenizer.h"

// char *str = "2 2 + 1 2 + *";  // (2 + 2) * (1 + 2) = 4 * 3 = 12
// Rules:
//     - Every token divided by at least one space (is it? what about 2 2+? is easy to detect)

Token *token(Arena *arena, TokenType type, String8 str) {
    Token *token = arena_alloc(arena, sizeof(Token), 8);
    *token = (Token) {.type = type, .str = str };
    return token;
}

Token *next_token(Arena *arena, String8 *s) {
    *s = str8_trim(*s);

    byte *token_str_ptr = arena_alloc(arena, TOKEN_MAX_SIZE, TOKEN_MAX_SIZE);
    String8 token_str = str8(token_str_ptr, 0);
    TokenType token_type = UNINITIALIZED;

    byte *ch_ptr = s->chars;
    while(!char_is_space(*ch_ptr) && !char_is_nullterm(*ch_ptr)) {
        byte c = *ch_ptr;
        TokenType char_type = UNINITIALIZED;

        if      (c == '+') { char_type = TOK_PLUS; }
        else if (c == '-') { char_type = TOK_MINUS; }
        else if (c == '*') { char_type =  TOK_MULTIPLY; }
        else if (c == '/') { char_type = TOK_DIVIDE; }
        else if (char_is_digit(c, Base10)) { char_type = TOK_NUMBER; }
        else { continue; }

        token_str = str8_concat(arena, token_str, str8_from_char(&c));

        if (token_type != UNINITIALIZED && char_type != token_type) {
            break;
        }
        token_type = char_type;

        ch_ptr += 1;
    }

    *s = str8_skip(*s, token_str.len);
    return token(arena, token_type, token_str);
}


