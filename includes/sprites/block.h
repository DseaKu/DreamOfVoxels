#ifndef BLOCK_H
#define BLOCK_H

#include "sprites/player.h"
#include "std_includes.h"

// #define MAX_BLOCKS 10000

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
  bool active;
  Vector3 position;
  BlockFaces faces;
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

#endif // BLOCK_H
