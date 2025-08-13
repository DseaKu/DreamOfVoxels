#ifndef BLOCK_H
#define BLOCK_H

#include "scenes/scene_3d_game.h"
#include "sprites/player.h"
#include "std_includes.h"
#include <stdint.h>

// Bitmask for block faces using a single uint8_t.
// If state is 0, the block is inactive.
// If state > 0, the block is active, and the bits represent the visible faces.
#define FACE_TOP (1 << 0)    // 0b000001
#define FACE_BOTTOM (1 << 1) // 0b000010
#define FACE_FRONT (1 << 2)  // 0b000100
#define FACE_BACK (1 << 3)   // 0b001000
#define FACE_LEFT (1 << 4)   // 0b010000
#define FACE_RIGHT (1 << 5)  // 0b100000

typedef struct Block {
  uint8_t state;
  Vector3 position;
} Block;

void RemoveBlock(Block blocks[PLAYGROUND_Z_MAX][PLAYGROUND_X_MAX][PLAYGROUND_Y_MAX]);

void UpdateAllBlockFaces(
    Block blocks[PLAYGROUND_Z_MAX][PLAYGROUND_X_MAX][PLAYGROUND_Y_MAX]);

void DrawCubeFace(
    Block blocks[PLAYGROUND_Z_MAX][PLAYGROUND_X_MAX][PLAYGROUND_Y_MAX]);

#endif // BLOCK_H