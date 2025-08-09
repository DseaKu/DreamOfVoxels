#ifndef BLOCK_H
#define BLOCK_H

#include "std_includes.h"

#define MAX_BLOCKS 10000

typedef struct Block {
  Vector3 position;
  bool active;
} Block;

Block CreatePlaneGround();

#endif // BLOCK_H
