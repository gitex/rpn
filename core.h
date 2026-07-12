#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

///////////////////////////////////////////////////////
//////////////////// Context //////////////////////////

#if UINTPTR_MAX == 0xffffffffffffffff
    #define PLATFORM_64BIT
#elif UINTPTR_MAX == 0xffffffff
    #define PLATFORM_32BIT
#else
    #error "Unsupported pointer size"
#endif  // PLATFORM

///////////////////////////////////////////////////////
//////////////////// Types ////////////////////////////

typedef int8_t        i8;
typedef int16_t       i16;
typedef int32_t       i32;
typedef int64_t       i64;

typedef uint8_t       u8;
typedef uint16_t      u16;
typedef uint32_t      u32;
typedef uint64_t      u64;

typedef float         f32;
typedef double        f64;
typedef unsigned char byte;
typedef uintptr_t     uptr;
typedef void*         ptr;
typedef size_t        usize;

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

#define null      NULL
#define nullptr   NULL
#define false     0
#define true      1

#define cast(T, v)   (*(T *)(v))

///////////////////////////////////////////////////////
///////////////// Size aliases ////////////////////////

#define Kilobytes(value)  ((value) * 1024LL)
#define Megabytes(value)  (Kilobytes((value)) * 1024LL)
#define Gigabytes(value)  (Megabytes((value)) * 1024LL)
#define Terabytes(value)  (Gigabytes((value)) * 1024LL)

///////////////////////////////////////////////////////
//////////////////// String ///////////////////////////


///////////////////////////////////////////////////////
//////////////////// Math /////////////////////////////

u64 pow_u64(u64 base, u64 exp);
#define pow2(x)  (x) * (x);
#define pow3(x)  (x) * (x) * (x);
#define pow4(x)  (x) * (x) * (x) * (x);

typedef enum {
    Base2 =  2,
    Base4 =  4,
    Base8 =  8,
    Base10 = 10,
    Base16 = 16,
} NumericBase;

///////////////////////////////////////////////////////
//////////////////// Binary ///////////////////////////

u32 next_pow2_32(u32 v);
u64 next_pow2_64(u64 v);

#ifdef PLATFORM_64BIT
    #define next_pow2 next_pow2_64
#elif PLATFORM_32BIT
    #define next_pow2 next_pow2_32
#else
    #error "Undefined platform"
#endif  // PLATFORM

///////////////////////////////////////////////////////

#define NotImplemented(s) (printf("%s is not implemented yet", (s)), exit(1))

# endif  // TYPES_H
