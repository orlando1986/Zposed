#ifndef COMMON_H
#define COMMON_H

# include <stdint.h>

#ifdef HAVE_STDINT_H
typedef uint8_t u1;
typedef uint16_t u2;
typedef uint32_t u4;
typedef uint64_t u8;
typedef int8_t s1;
typedef int16_t s2;
typedef int32_t s4;
typedef int64_t s8;
#else
typedef unsigned char u1;
typedef unsigned short u2;
typedef unsigned int u4;
typedef unsigned long long u8;
typedef signed char s1;
typedef signed short s2;
typedef signed int s4;
typedef signed long long s8;
#endif

#define LIKELY(x)       __builtin_expect((x), true)
#define UNLIKELY(x)     __builtin_expect((x), false)
#define MANAGED __attribute__((aligned(4)))
#define ALWAYS_INLINE  __attribute__ ((always_inline))
#define PACKED(x) __attribute__ ((__aligned__(x), __packed__))

#endif
