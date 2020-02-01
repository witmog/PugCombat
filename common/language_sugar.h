#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

typedef int64_t i64;
typedef int32_t i32;
typedef int16_t i16;
typedef int8_t  i8;

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef uint64_t b64;
typedef uint32_t b32;
typedef uint16_t b16;
typedef uint8_t  b8;
#define true  1
#define false 0

typedef float   f32;
typedef double  f64;

#define global   static
#define internal static
#define persist  static

#define signum(x) ((x > 0) ? 1 : ((x < 0) ? -1 : 0))
#define clamp(val, min, max) ((val > max) ? max : ((val < min) ? min : val))
