#ifndef STRING_H
#define STRING_H

#include "core.h"

typedef struct String8 {
    u8 *str;
    u64 size;
} String8;

i8 char_is_space(u8 c);
i8 char_is_upper(u8 c);
i8 char_is_lower(u8 c);
i8 char_is_alpha(u8 c);
i8 char_is_slash(u8 c);
i8 char_is_digit(u8 c, u32 base);
u8 lower_from_char(u8 c);
u8 upper_from_char(u8 c);

u64 cstr8_length(u8 *c);

#define s(S)         str8_lit((S))
#define str8_lit(S)  str8((u8*)(S) sizeof((S)) - 1)

String8 str8(byte *str, u64 size);

#endif // STRING_H
