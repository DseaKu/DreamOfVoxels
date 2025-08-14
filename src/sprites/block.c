#include "sprites/block.h"
#include "raymath.h"
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

void RemoveBlock(
    Block blocks[PLAYGROUND_X_MAX][PLAYGROUND_Y_MAX][PLAYGROUND_Z_MAX],
    Player *player) {
  Vector3 camera_pos = player->camera.position;
  Vector3 camera_dir = Vector3Normalize(
      Vector3Subtract(player->camera.target, player->camera.position));

  for (float i = 0; i < PLAYER_REACH; i += 0.1f) {
    Vector3 check_pos =
        Vector3Add(camera_pos, Vector3Scale(camera_dir, i));
    int x = (int)round(check_pos.x);
    int y = (int)round(check_pos.y);
    int z = (int)round(check_pos.z);

    if (x < 0 || x >= PLAYGROUND_X_MAX || y < 0 || y >= PLAYGROUND_Y_MAX ||
        z < 0 || z >= PLAYGROUND_Z_MAX) {
      continue;
    }

    if (blocks[x][y][z].is_active) {
      blocks[x][y][z].is_active = false;
      return;
    }
  }
}

void AddBlock(
    Block blocks[PLAYGROUND_X_MAX][PLAYGROUND_Y_MAX][PLAYGROUND_Z_MAX],
    Player *player) {
  Vector3 camera_pos = player->camera.position;
  Vector3 camera_dir = Vector3Normalize(
      Vector3Subtract(player->camera.target, player->camera.position));

  Vector3 last_pos = {0};

  for (float i = 0; i < PLAYER_REACH; i += 0.1f) {
    Vector3 check_pos =
        Vector3Add(camera_pos, Vector3Scale(camera_dir, i));
    int x = (int)round(check_pos.x);
    int y = (int)round(check_pos.y);
    int z = (int)round(check_pos.z);

    if (x < 0 || x >= PLAYGROUND_X_MAX || y < 0 || y >= PLAYGROUND_Y_MAX ||
        z < 0 || z >= PLAYGROUND_Z_MAX) {
      continue;
    }

    if (blocks[x][y][z].is_active) {
      int place_x = (int)round(last_pos.x);
      int place_y = (int)round(last_pos.y);
      int place_z = (int)round(last_pos.z);

      if (place_x >= 0 && place_x < PLAYGROUND_X_MAX && place_y >= 0 &&
          place_y < PLAYGROUND_Y_MAX && place_z >= 0 &&
          place_z < PLAYGROUND_Z_MAX) {
        blocks[place_x][place_y][place_z].is_active = true;
      }
      return;
    }
    last_pos = check_pos;
  }
}