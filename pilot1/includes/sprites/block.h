#ifndef BLOCK_H
#define BLOCK_H

#include "scenes/scene_3d_game.h"
#include "std_includes.h"
#include <raylib.h>
#include <stdint.h>

typedef struct Block {
  bool is_visible;
  bool is_active;
  Vector3 position;
} Block;

typedef struct SearchScope {
  const u8 x1;
  const u8 x2;
  const u8 y1;
  const u8 y2;
  const u8 z1;
  const u8 z2;
} SearchScope;

void InitBlocks(
    Block blocks[PLAYGROUND_X_MAX][PLAYGROUND_Y_MAX][PLAYGROUND_Z_MAX]);

void RemoveBlock(
    Block blocks[PLAYGROUND_X_MAX][PLAYGROUND_Y_MAX][PLAYGROUND_Z_MAX]);

void UpdateAllBlockFaces(
    Block blocks[PLAYGROUND_X_MAX][PLAYGROUND_Y_MAX][PLAYGROUND_Z_MAX],
    SearchScope scope);

void DrawCubeFace(
    Block blocks[PLAYGROUND_X_MAX][PLAYGROUND_Y_MAX][PLAYGROUND_Z_MAX],
    Model model);

#endif // BLOCK_H
