#include "core/block.h"
#include "scenes/scene_3d_game.h"
#include <raylib.h>

void InitBlocks(
    Block blocks[PLAYGROUND_X_MAX][PLAYGROUND_Y_MAX][PLAYGROUND_Z_MAX]) {

  for (u32 x = 0; x < PLAYGROUND_X_MAX; x++) {
    for (u32 y = 0; y < PLAYGROUND_Y_MAX; y++) {
      for (u32 z = 0; z < PLAYGROUND_Z_MAX; z++) {
        blocks[x][y][z].is_visible = false;
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
        if (blocks[x][y][z].is_visible) {
          DrawModel(model, blocks[x][y][z].position, 1.0f, WHITE);
        }
      }
    }
  }
}

void RemoveBlock(
    Block blocks[PLAYGROUND_X_MAX][PLAYGROUND_Y_MAX][PLAYGROUND_Z_MAX]) {}

void UpdateAllBlockFaces(
    Block blocks[PLAYGROUND_X_MAX][PLAYGROUND_Y_MAX][PLAYGROUND_Z_MAX],
    SearchScope scope) {
  for (u32 x = scope.x1; x < scope.x2; x++) {
    for (u32 y = scope.y1; y < scope.y2; y++) {
      for (u32 z = scope.z1; z < scope.z2; z++) {
        if (!blocks[x][y][z].is_active) {
          blocks[x][y][z].is_visible = false;
          continue;
        }

        // Check for neighbors in all 6 directions
        bool has_left_neighbor = (x > 0 && blocks[x - 1][y][z].is_active);
        bool has_right_neighbor =
            (x < PLAYGROUND_X_MAX - 1 && blocks[x + 1][y][z].is_active);
        bool has_bottom_neighbor = (y > 0 && blocks[x][y - 1][z].is_active);
        bool has_top_neighbor =
            (y < PLAYGROUND_Y_MAX - 1 && blocks[x][y + 1][z].is_active);
        bool has_back_neighbor = (z > 0 && blocks[x][y][z - 1].is_active);
        bool has_front_neighbor =
            (z < PLAYGROUND_Z_MAX - 1 && blocks[x][y][z + 1].is_active);

        if (has_left_neighbor && has_right_neighbor && has_bottom_neighbor &&
            has_top_neighbor && has_back_neighbor && has_front_neighbor) {
          blocks[x][y][z].is_visible = false;
        } else {
          blocks[x][y][z].is_visible = true;
        }
      }
    }
  }
}
