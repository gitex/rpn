#include <string.h>
#include <stdio.h>

#include "core.h"
#include "string.h"


#define ASCII_LOWER_UPPER_DIFF 32

String8 str8(byte *str, u64 size) {
    return (String8) { .chars = str, .size = size };
}

String8 str8_concat(Arena *arena, String8 s1, String8 s2) {
    u64 size = s1.size + s2.size;
    byte *ptr = arena_alloc(arena, size, 4);

    memcpy(ptr, s1.chars, s1.size);
    memcpy(ptr + s1.size, s2.chars, s2.size);

    return str8(ptr, size);
}

void str8_print(const String8 *s) {
    printf("%.*s\n", (int)s->size, s->chars);
}

String8 str8_skip(const String8 s, u64 size) {
    if (size > s.size) return str8_empty;
    return str8(s.chars + size, s.size - size);
}

// QUESTION: what if we want to trim only spaces or only new line chars?
String8 str8_trim(String8 s) {
    while(char_is_space(*s.chars)) {
        s.chars += 1;
        s.size--;
    }
    return s;
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

