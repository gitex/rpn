#include "string.h"

#define TOKEN_MAX_SIZE 64

typedef enum {
    TOK_UINT,
    TOK_OPERATION,
    TOK_UNKNOWN,
} TokenTag;

/* Operands and operations */

typedef i64 Opnd;  // Operand

typedef Opnd (*Nullary)();
typedef Opnd (*Unary)  (Opnd);
typedef Opnd (*Binary) (Opnd, Opnd);
typedef Opnd (*Ternary)(Opnd, Opnd, Opnd);

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


Opnd op_add(Opnd a, Opnd b);
Opnd op_minus(Opnd a, Opnd b);
Opnd op_multiply(Opnd a, Opnd b);
Opnd op_divide(Opnd a, Opnd b);
Opnd op_modulo(Opnd a, Opnd b);
Opnd op_min2(Opnd a, Opnd b);
Opnd op_max2(Opnd a, Opnd b);
Opnd op_min3(Opnd a, Opnd b, Opnd c);
Opnd op_max3(Opnd a, Opnd b, Opnd c);

static const Op ops[] = {
    {.word="+",    .arity=2, .fn = { .binary = op_add } },
    {.word="-",    .arity=2, .fn = { .binary = op_minus } },
    {.word="*",    .arity=2, .fn = { .binary = op_multiply } },
    {.word="/",    .arity=2, .fn = { .binary = op_divide } },
    {.word="%",    .arity=2, .fn = { .binary = op_modulo } },
    {.word="min2", .arity=2, .fn = { .binary = op_min2 } },
    {.word="max2", .arity=2, .fn = { .binary = op_max2 } },

    {.word="min3", .arity=3, .fn = { .ternary = op_min3 } },
    {.word="max3", .arity=3, .fn = { .ternary = op_max3 } },
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

/* Result */

typedef struct {
    Opnd operand;
    String8 err_msg;
} Result;

/* Methods */

Token *next_token(Arena *arena, String8 *s);

Result calc_expression(Arena *arena, String8 expr);

