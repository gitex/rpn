#include "core.h"


u32 next_pow2_32(u32 v) {
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    return v;
}

u64 next_pow2_64(u64 v) {
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v |= v >> 32;
    v++;
    return v;
}

u64 pow_u64(u64 base, u64 exp) {
    if (exp == 0) {
        return 1;
    }

    if (base == 2) {
        return 1 << exp;
    }

    u64 res = 1;

    while (exp != 0) {
        if (exp & 1) {
            res *= base;
        }
        base *= base;
        exp >>= 1;
    }

    return res;
}
