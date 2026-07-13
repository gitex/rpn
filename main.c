#include <stdio.h>
#include <assert.h>
#include <unistd.h>

#include "core.h"
#include "string.h"
#include "tokenizer.h"
#include "console.h"

#define INPUT_PREFIX " > "


i64 run_interpreter(void) {
    char cur_input[1024];

    Arena *arena = arena_init(NULL, Kilobytes(32));

    String8 expr;

    u8 quit = false;
    while(quit != true) {
        term_print(str8_lit(INPUT_PREFIX));

        fgets(cur_input, sizeof(cur_input), stdin);
        expr = str8((byte*)cur_input, strlen(cur_input) - 1); // brrrr, nullterm

        if (str8_match(expr, str8_lit("q")) == 0) {
            quit = true;
            break;
        }

        Result result = calc_expression(arena, expr);

        if (result.err_msg.len > 0) {
            term_print_colorized(result.err_msg, FG_RED);
            continue;
        }

        printf("%lu\n", result.operand);
        arena_reset(arena);
    }

    return 0;
}

int main(void) {
    run_interpreter();

    return 0;
}
