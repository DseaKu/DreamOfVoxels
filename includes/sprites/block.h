#ifndef BLOCK_H
#define BLOCK_H

#include "sprites/player.h"
#include "std_includes.h"
#include <stdint.h>

// #define MAX_BLOCKS 10000

typedef enum { TOP, BOTTOM, FRONT, BACK, LEFT, RIGHT } CubeFace;

// Bitmask for block faces using a single uint8_t.
// If state is 0, the block is inactive.
// If state > 0, the block is active, and the bits represent the visible faces.
#define FACE_TOP    (1 << 0) // 0b000001
#define FACE_BOTTOM (1 << 1) // 0b000010
#define FACE_FRONT  (1 << 2) // 0b000100
#define FACE_BACK   (1 << 3) // 0b001000
#define FACE_LEFT   (1 << 4) // 0b010000
#define FACE_RIGHT  (1 << 5) // 0b100000

typedef struct Block {
  uint8_t state;
  Vector3 position;
} Block;

void InitBlocks(int z_max, int x_max, int y_max,
                Block blocks[z_max][x_max][y_max], float block_size);

void RemovePlacement(float max_placement_distance, Player *player, int z_max,
                     int x_max, int y_max, Block blocks[z_max][x_max][y_max],
                     float block_size);

void DrawBlocks(int z_max, int x_max, int y_max,
                Block blocks[z_max][x_max][y_max], float block_size);

void UpdateAllBlockFaces(int z_max, int x_max, int y_max,
                         Block blocks[z_max][x_max][y_max]);

void DrawCubeFace(CubeFace face, Vector3 position, float size, Color color);

#endif // BLOCK_H