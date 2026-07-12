#ifndef STRING_H
#define STRING_H

#include <string.h>

#include "core.h"
#include "arena.h"

typedef struct String8 {
    byte *chars;
    u64 len;
} String8;

#define s(S)               str8_lit((S))
#define str8_lit(S)        str8((u8*)(S), strlen((S)))
#define str8_from_char(c)  str8((u8*)(c), sizeof(u8))
#define str8_empty         str8_lit("")
String8 str8(byte *str, u64 length);
#define str8_alloc_cstr(a, chars) str8_alloc(a, (u8 *)chars, sizeof(chars) - 1)
String8 *str8_alloc(Arena *arena, char *chars, u64 length);
String8 *str8_from_cstr(Arena* arena, char *chars);

/* Chars */
i8 char_is_space(u8 c);
i8 char_is_upper(u8 c);
i8 char_is_lower(u8 c);
i8 char_is_alpha(u8 c);
i8 char_is_slash(u8 c);
i8 char_is_digit(u8 c, u32 base);
u8 lower_from_char(u8 c);
u8 upper_from_char(u8 c);

#define char_is_nullterm(c) ((c) == '\0')

/* Modifying */
String8 str8_reverse(String8 s);
String8 str8_trim(String8 s);

/* Match */
i32 str8_match(String8 a, String8 b);
i8 str8_starts_with(String8 str, String8 *prefix);
i8 str8_ends_with(String8 str, String8 *suffix);

/* Slicing */
String8 str8_slice(String8 str, u64 start, u64 end);
String8 str8_prefix(String8 s, u64 size);
String8 str8_skip(String8 s, u64 size);
String8 str8_chop(String8 s, u64 size);

/* Formating / copying */
String8 str8_concat(Arena* arena, String8 s1, String8 s2);
String8 str8_copy(Arena* arena, String8 s);

/* Conversions */
u32 u32_from_str8(String8 str, u32 base);
u64 u64_from_str8(String8 str, u32 base);
i32 i32_from_str8(String8 str, u32 base);
i64 i64_from_str8(String8 str, u32 base);

void str8_print(const String8 *s);
u64 cstr8_length(u8 *c);
u64 str8_is_u64(String8 s, u32 base);

#endif // STRING_H
