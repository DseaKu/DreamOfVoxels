#ifndef BLOCK_H
#define BLOCK_H

#include "scenes/scene_3d_game.h"
#include "sprites/player.h"
#include "std_includes.h"
#include <raylib.h>
#include <stdint.h>

typedef struct Block {
  bool is_visible;
  bool is_active;
  Vector3 position;
} Block;

void InitBlocks(
    Block blocks[PLAYGROUND_X_MAX][PLAYGROUND_Y_MAX][PLAYGROUND_Z_MAX]);

void RemoveBlock(
    Block blocks[PLAYGROUND_X_MAX][PLAYGROUND_Y_MAX][PLAYGROUND_Z_MAX],
    Player *player);

void AddBlock(Block blocks[PLAYGROUND_X_MAX][PLAYGROUND_Y_MAX][PLAYGROUND_Z_MAX],
              Player *player);

#endif // BLOCK_H