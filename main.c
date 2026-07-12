#include <stdio.h>
#include <assert.h>

#include "string.h"
#include "tokenizer.h"


int main() {
    String8 str = str8_lit("4 1 %");
    Opnd result = calc_expression(str);
    printf("%lu\n", result);
    return 0;
}
