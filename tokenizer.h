#include "string.h"

#define TOKEN_MAX_SIZE 64

typedef enum {
    TOK_NUMBER = 0,
    TOK_PLUS,
    TOK_MINUS,
    TOK_MULTIPLY,
    TOK_DIVIDE,
} TokenType;

typedef struct {
    TokenType type;
    String8 str;
} Token;


Token *next_token(Arena *arena, String8 *s);
