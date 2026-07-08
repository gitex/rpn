#include "core.h"
#include "string.h"


#define STRING_MIN_CAPACITY 2
#define STRING_ALIGN        8
#define ASCII_LOWER_UPPER_DIFF 32

String8 str8(byte *str, u64 size) {
    return (String8) { .str = str, .size = size };
}

i8 char_is_space(u8 c) {
    // TODO: don't remember all of them, need to add it later
    return c == ' ' || c == '\n' || c == '\t' || c == '\r';
}

i8 char_is_digit(u8 c, u32 base) {
    switch (base) {
        case 2:
            return c == '0' || c == '1';
        case 4:
            return c >= '0' && c <= '4';
        case 8:
            return c >= '0' && c <= '8';
        case 10:
            return c >= '0' && c <= '9';
        case 16:
            return (c >= '0' && c <= '9')
                || (c >= 'a' && c <= 'f')
                || (c >= 'A' && c <= 'F');
    }
    return -1;
}

i8 char_is_lower(u8 c) {
    return (c >= 'a' && c <= 'z');
}

i8 char_is_upper(u8 c) {
    return (c >= 'A' && c <= 'Z');
}

i8 char_is_alpha(u8 c) {
    return char_is_lower(c) || char_is_upper(c);
}

i8 char_is_slash(u8 c) {
    return c == '/' || c == '\\';
}

u8 lower_from_char(u8 c) { return c + ASCII_LOWER_UPPER_DIFF; }
u8 upper_from_char(u8 c) { return c - ASCII_LOWER_UPPER_DIFF; }

