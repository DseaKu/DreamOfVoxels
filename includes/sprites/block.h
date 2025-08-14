#ifndef BLOCK_H
#define BLOCK_H

#include "scenes/scene_3d_game.h"
#include "std_includes.h"
#include <raylib.h>
#include <stdint.h>

typedef struct Block {
  bool is_active;
  Vector3 position;
} Block;

typedef struct SearchScope {

} SearchScope;

void InitBlocks(
    Block blocks[PLAYGROUND_Z_MAX][PLAYGROUND_X_MAX][PLAYGROUND_Y_MAX]);

void RemoveBlock(
    Block blocks[PLAYGROUND_Z_MAX][PLAYGROUND_X_MAX][PLAYGROUND_Y_MAX]);

void UpdateAllBlockFaces(
    Block blocks[PLAYGROUND_Z_MAX][PLAYGROUND_X_MAX][PLAYGROUND_Y_MAX]);

void DrawCubeFace(
    Block blocks[PLAYGROUND_Z_MAX][PLAYGROUND_X_MAX][PLAYGROUND_Y_MAX],
    Model model);

#endif // BLOCK_H
