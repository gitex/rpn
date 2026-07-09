#include "core.h"
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
    TokenType type = -1;

    byte *start = s->chars;
    while(!char_is_space(*start)) {
        byte c = *s->chars;
        TokenType char_type = -1;

        if      (c == '+') { char_type = TOK_PLUS; }
        else if (c == '-') { char_type = TOK_MINUS; }
        else if (c == '*') { char_type =  TOK_MULTIPLY; }
        else if (c == '/') { char_type = TOK_DIVIDE; }
        else if (char_is_digit(c, Base10)) { char_type = TOK_NUMBER; }
        else { return null; }

        token_str = str8_concat(arena, token_str, str8_from_char(&c));

        if (type != -1 && char_type != type) {
            break;
        }

        type = char_type;
        start += 1;
    }

    *s = str8_skip(*s, token_str.size);
    return token(arena, type, token_str);
}


