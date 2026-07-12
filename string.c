#include <string.h>
#include <stdio.h>
#include <math.h>

#include "core.h"
#include "string.h"
#include "arena.h"


#define ASCII_LOWER_UPPER_DIFF 32

// 0 - 9  => 48 - 57
#define ascii_char_to_digit(c) ((c) - 48);

String8 str8(u8 *chars, u64 length) {
    return (String8) { .chars = chars, .len = length };
}

String8 *str8_alloc(Arena *arena, const char *chars, u64 length) {
    byte *ptr = arena_alloc(arena, length + sizeof(String8), 8);
    byte *data_ptr = ptr + sizeof(String8);
    memcpy(data_ptr, chars, length);

    *(String8 *)ptr = str8(data_ptr, length);
    return (String8 *)ptr;
}

String8 *str8_from_cstr(Arena* arena, const char *chars) {
    return str8_alloc(arena, chars, strlen(chars));
}

/////////////////////////////////////////////////////////////
// String modifications

String8 str8_concat(Arena *arena, String8 s1, String8 s2) {
    u64 len = s1.len + s2.len;
    u8 *ptr = arena_alloc(arena, len, 4);
    if (!ptr) return str8_empty;

    memcpy(ptr, s1.chars, s1.len);
    memcpy(ptr + s1.len, s2.chars, s2.len);

    return str8(ptr, len);
}


String8 str8_skip(const String8 s, u64 size) {
    if (size > s.len) return str8_empty;
    return str8(s.chars + size, s.len - size);
}

// QUESTION: what if we want to trim only spaces or only new line chars? flags? trim_any_of?
String8 str8_trim(String8 s) {
    while(char_is_space(*s.chars)) {
        s.chars += 1;
        s.len--;
    }
    return s;
}

String8 str8_reverse(String8 s) {
    for (size_t i = 0; i < s.len / 2; i++) {
        u8 *l = s.chars + i,
           *r = s.chars + s.len - 1 - i;

        u8 tmp = *l;
        *l = *r;
        *r = tmp;
    }

    return s;
}

/////////////////////////////////////////////////////////////
// Matching

// TODO: add flags to match with different options
i32 str8_match(String8 a, String8 b) {
    size_t min_len = (a.len > b.len) ? b.len : a.len;

    for (size_t i = 0; i < min_len; i++) {
        u8 lch = a.chars[i],
           rch = b.chars[i];

        if (lch > rch) {
            return 1;
        } else if (lch < rch) {
            return -1;
        }
    }

    if (a.len > b.len) {
        return 1;
    } else if (a.len < b.len) {
        return -1;
    }

    return 0;
}


/////////////////////////////////////////////////////////////
// Conversion Int <=> String

u64 u64_from_str8(String8 s, u32 base) {
    u64 result = 0;

    // TODO: add other bases
    switch (base) {
        case Base10:
            for(size_t i = 0; i < s.len; i++) {
                u64 n = ascii_char_to_digit(s.chars[i]);
                u64 exp = s.len - 1 - i;
                result += (n * pow_u64(10, exp));
            }
            break;
        default:
            NotImplemented("u64_from_string on other bases");
    }

    return result;
}

/////////////////////////////////////////////////////////////
// Other

i32 str8_is_empty(const String8 s) {
    return s.len == 0;
}


u64 str8_is_u64(String8 s, u32 base) {
    switch (base) {
        case Base10:
            for (usize i = 0; i < s.len; i++) {
                if (!char_is_digit(s.chars[i], Base10)) {
                    return false;
                }
            }
            return true;

        default:
            NotImplemented("str8_is_numeric for other bases not implemented yet");
    }
}

// TODO: this does not look like proper formating to me
void str8_print(const String8 *s) {
    printf("%.*s\n", (int)s->len, s->chars);
}

/////////////////////////////////////////////////////////////
// Chars

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

