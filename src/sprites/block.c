#include "sprites/block.h"
#include "scenes/scene_3d_game.h"
#include <raylib.h>

void InitBlocks(
    Block blocks[PLAYGROUND_Z_MAX][PLAYGROUND_X_MAX][PLAYGROUND_Y_MAX]) {

  for (u32 z = 0; z < PLAYGROUND_Z_MAX; z++) {
    for (u32 x = 0; x < PLAYGROUND_X_MAX; x++) {
      for (u32 y = 0; y < PLAYGROUND_Y_MAX; y++) {
        blocks[z][x][y].is_active = true;
        blocks[z][x][y].position = (Vector3){(float)x, (float)y, (float)z};
      }
    }
  }
}

void DrawCubeFace(
    Block blocks[PLAYGROUND_Y_MAX][PLAYGROUND_X_MAX][PLAYGROUND_Y_MAX],
    Model model) {

  for (u32 z = 0; z < PLAYGROUND_Z_MAX; z++) {
    for (u32 x = 0; x < PLAYGROUND_X_MAX; x++) {
      for (u32 y = 0; y < PLAYGROUND_Y_MAX; y++) {
        DrawModel(model, blocks[z][x][y].position, 1.0f, WHITE);
      }
    }
  }
}

void RemoveBlock(
    Block blocks[PLAYGROUND_Z_MAX][PLAYGROUND_X_MAX][PLAYGROUND_Y_MAX]) {}

void UpdateAllBlockFaces(
    Block blocks[PLAYGROUND_Z_MAX][PLAYGROUND_X_MAX][PLAYGROUND_Y_MAX]) {
  for (u32 z = 0; z < PLAYGROUND_Z_MAX; z++) {
    for (u32 x = 0; x < PLAYGROUND_X_MAX; x++) {
      for (u32 y = 0; y < PLAYGROUND_Y_MAX; y++) {
      }
    }
  }
}
