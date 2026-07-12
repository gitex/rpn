#include <stdio.h>
#include <assert.h>

#include "string.h"
#include "tokenizer.h"


int main() {
    String8 str = str8_lit("5 0 /");
    Result result = calc_expression(str);
    printf("%lu\n", result.value.u);
    return 0;
}
