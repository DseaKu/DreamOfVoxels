#include "sprites/block.h"
#include "scenes/scene_3d_game.h"
#include <raylib.h>

void InitBlocks(
    Block blocks[PLAYGROUND_X_MAX][PLAYGROUND_Y_MAX][PLAYGROUND_Z_MAX]) {

  for (u32 x = 0; x < PLAYGROUND_X_MAX; x++) {
    for (u32 y = 0; y < PLAYGROUND_Y_MAX; y++) {
      for (u32 z = 0; z < PLAYGROUND_Z_MAX; z++) {
        blocks[x][y][z].is_active = true;
        blocks[x][y][z].position = (Vector3){(float)x, (float)y, (float)z};
      }
    }
  }
}

void DrawCubeFace(
    Block blocks[PLAYGROUND_X_MAX][PLAYGROUND_Y_MAX][PLAYGROUND_Z_MAX],
    Model model) {

  for (u32 x = 0; x < PLAYGROUND_X_MAX; x++) {
    for (u32 y = 0; y < PLAYGROUND_Y_MAX; y++) {
      for (u32 z = 0; z < PLAYGROUND_Z_MAX; z++) {
        DrawModel(model, blocks[x][y][z].position, 1.0f, WHITE);
      }
    }
  }
}

void RemoveBlock(
    Block blocks[PLAYGROUND_X_MAX][PLAYGROUND_Y_MAX][PLAYGROUND_Z_MAX]) {}

void UpdateAllBlockFaces(
    Block blocks[PLAYGROUND_X_MAX][PLAYGROUND_Y_MAX][PLAYGROUND_Z_MAX],
    SearchScope scope) {
  for (u32 x = 0; x < PLAYGROUND_X_MAX; x++) {
    for (u32 y = 0; y < PLAYGROUND_Y_MAX; y++) {
      for (u32 z = 0; z < PLAYGROUND_Z_MAX; z++) {
      }
    }
  }
}
