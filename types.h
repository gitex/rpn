#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <stddef.h>

typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float    f32;
typedef double   f64;

typedef uintptr_t uptr;


#define i8_min    INT8_MIN;
#define i16_min   INT16_MIN;
#define i32_min   INT32_MIN;
#define i64_min   INT64_MIN;

#define i8_max    INT8_MAX;
#define i16_max   INT16_MAX;
#define i32_max   INT32_MAX;
#define i64_max   INT64_MAX;

#define u8_max    UINT8_MAX;
#define u16_max   UINT16_MAX;
#define u32_max   UINT32_MAX;
#define u64_max   UINT64_MAX;


# endif  // TYPES_H
