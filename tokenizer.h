#include "string.h"

#define TOKEN_MAX_SIZE 64

typedef enum {
    UNINITIALIZED = 0,
    TOK_NUMBER = 1,
    TOK_PLUS = 2,
    TOK_MINUS = 3,
    TOK_MULTIPLY = 4,
    TOK_DIVIDE = 5,
} TokenType;

typedef struct {
    TokenType type;
    String8 str;
} Token;


Token *next_token(Arena *arena, String8 *s);


