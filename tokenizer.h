#include "string.h"

#define TOKEN_MAX_SIZE 64

typedef enum {
    TOK_UINT,
    TOK_OPERATION,
} TokenTag;

typedef const u64 (*Nullary)();
typedef const u64 (*Unary)(u64);
typedef const u64 (*Binary)(u64, u64);
typedef const u64 (*Ternary)(u64, u64, u64);


typedef struct {
    const char *word;
    u8 arity;
    union {
        Nullary nullary;
        Unary unary;
        Binary binary;
        Ternary ternary;
    } fn;
} Op;


const u64 add(u64 a, u64 b);
const u64 minus(u64 a, u64 b);
const u64 multiply(u64 a, u64 b);
const u64 divide(u64 a, u64 b);
const u64 min2(u64 a, u64 b);
const u64 max2(u64 a, u64 b);
const u64 min3(u64 a, u64 b, u64 c);
const u64 max3(u64 a, u64 b, u64 c);


static Op ops[] = {
    (Op) {.word="+",    .arity=2, .fn = { .binary = add } },
    (Op) {.word="-",    .arity=2, .fn = { .binary = minus } },
    (Op) {.word="*",    .arity=2, .fn = { .binary = multiply } },
    (Op) {.word="/",    .arity=2, .fn = { .binary = divide } },
    (Op) {.word="min2", .arity=2, .fn = { .binary = min2 } },
    (Op) {.word="max2", .arity=2, .fn = { .binary = max2 } },
};

static const usize ops_count = sizeof(ops) / sizeof(Op);


typedef struct {
    String8 str;
    TokenTag tag;
    union {
        Op op;
        u64 u;
    } value;
} Token;


Token *next_token(Arena *arena, String8 *s);


