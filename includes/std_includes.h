#ifndef STD_INCLUDES_H
#define STD_INCLUDES_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef struct s16Vector2D {
  s16 x;
  s16 z;
} s16Vector2D;
typedef struct s32Vector2D {
  s32 x;
  s32 z;
} s32Vector2D;
typedef struct s64Vector2D {
  s64 x;
  s64 z;
} s64Vector2D;
typedef struct s64Vector3D {
  s64 x;
  s64 z;
  s64 y;
} s64Vector3D;
#endif // !STD_INCLUDES_H_offset
