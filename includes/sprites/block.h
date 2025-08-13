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

void CreatePlaneGround(Block *blocks, int *blockCount, int y_max, int x_max);
void BlockPlacment(float max_placment_distance, Player *player, Block *blocks,
                   int *blockCount, float block_size);

void DrawBlocks(Block *blocks, int block_count, float block_size);

void UpdateAllBlockFaces(Block *blocks, int blockCount);

#endif // BLOCK_H
