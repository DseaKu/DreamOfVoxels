#ifndef BLOCK_H
#define BLOCK_H

#include "sprites/player.h"
#include "std_includes.h"

#define MAX_BLOCKS 10000

typedef enum { TOP, BOTTOM, FRONT, BACK, LEFT, RIGHT } CubeFace;

typedef struct BlockFaces {
  bool TOP;
  bool BOTTOM;
  bool FRONT;
  bool BACK;
  bool LEFT;
  bool RIGHT;
} BlockFaces;

typedef struct Block {
  Vector3 position;
  bool active;
  BlockFaces faces;
} Block;

void RemovePlacment(float max_placment_distance, Player *player, Block *blocks,
                    float block_size, int y_max, int x_max, int z_max);

void DrawBlocks(Block *blocks, float block_size, int y_max, int x_max,
                int z_max);

void UpdateAllBlockFaces(Block *blocks, int y_max, int x_max, int z_max);

#endif // BLOCK_H
