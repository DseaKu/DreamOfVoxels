#include "sprites/block.h"
#include <raylib.h>
#include <stdbool.h>

void InitBlocks(int y_max, int x_max, int z_max,
                Block blocks[y_max][x_max][z_max], float block_size) {
  for (int y = 0; y < y_max; y++) {
    for (int x = 0; x < x_max; x++) {
      for (int z = 0; z < z_max; z++) {
        blocks[y][x][z].active = true;
        blocks[y][x][z].position =
            (Vector3){(x - x_max / 2) * block_size, y * block_size,
                      (z - z_max / 2) * block_size};
      }
    }
  }
}

void RemovePlacement(float max_placement_distance, Player *player, int y_max,
                     int x_max, int z_max,
                     Block blocks[y_max][x_max][z_max], float block_size) {

  Vector3 direction = Vector3Normalize(
      Vector3Subtract(player->camera.target, player->camera.position));
  Ray ray = {player->camera.position, direction};

  RayCollision closestCollision = {0};
  closestCollision.distance = max_placement_distance;
  closestCollision.hit = false;

  int hit_y = -1, hit_x = -1, hit_z = -1;

  for (int y = 0; y < y_max; y++) {
    for (int x = 0; x < x_max; x++) {
      for (int z = 0; z < z_max; z++) {
        if (!blocks[y][x][z].active)
          continue;

        BoundingBox bb = {
            (Vector3){blocks[y][x][z].position.x - block_size / 2,
                      blocks[y][x][z].position.y - block_size / 2,
                      blocks[y][x][z].position.z - block_size / 2},
            (Vector3){blocks[y][x][z].position.x + block_size / 2,
                      blocks[y][x][z].position.y + block_size / 2,
                      blocks[y][x][z].position.z + block_size / 2}};
        RayCollision collision = GetRayCollisionBox(ray, bb);

        if (collision.hit && collision.distance < closestCollision.distance) {
          closestCollision = collision;
          hit_y = y;
          hit_x = x;
          hit_z = z;
        }
      }
    }
  }

  if (hit_y != -1) {
    blocks[hit_y][hit_x][hit_z].active = false;
  }
}
void DrawBlocks(int y_max, int x_max, int z_max,
                Block blocks[y_max][x_max][z_max], float block_size) {
  for (int y = 0; y < y_max; y++) {
    for (int x = 0; x < x_max; x++) {
      for (int z = 0; z < z_max; z++) {
        if (blocks[y][x][z].active) {
          if (blocks[y][x][z].faces.TOP)
            DrawCube(blocks[y][x][z].position, block_size, block_size,
                     block_size, RED);
          if (blocks[y][x][z].faces.BOTTOM)
            DrawCube(blocks[y][x][z].position, block_size, block_size,
                     block_size, GREEN);
          if (blocks[y][x][z].faces.FRONT)
            DrawCube(blocks[y][x][z].position, block_size, block_size,
                     block_size, BLUE);
          if (blocks[y][x][z].faces.BACK)
            DrawCube(blocks[y][x][z].position, block_size, block_size,
                     block_size, YELLOW);
          if (blocks[y][x][z].faces.LEFT)
            DrawCube(blocks[y][x][z].position, block_size, block_size,
                     block_size, PURPLE);
          if (blocks[y][x][z].faces.RIGHT)
            DrawCube(blocks[y][x][z].position, block_size, block_size,
                     block_size, ORANGE);

          DrawCubeWires(blocks[y][x][z].position, block_size, block_size,
                        block_size, MAROON);
        }
      }
    }
  }
}

void UpdateAllBlockFaces(int y_max, int x_max, int z_max,
                         Block blocks[y_max][x_max][z_max]) {
  for (int y = 0; y < y_max; y++) {
    for (int x = 0; x < x_max; x++) {
      for (int z = 0; z < z_max; z++) {

        // If the block is not active, none of its faces should be rendered.
        if (!blocks[y][x][z].active) {
          blocks[y][x][z].faces =
              (BlockFaces){false, false, false, false, false, false};
          continue;
        }

        // Check Top Face (y + 1)
        if (y + 1 >= y_max || !blocks[y + 1][x][z].active) {
          blocks[y][x][z].faces.TOP = true;
        } else {
          blocks[y][x][z].faces.TOP = false;
        }

        // Check Bottom Face (y - 1)
        if (y - 1 < 0 || !blocks[y - 1][x][z].active) {
          blocks[y][x][z].faces.BOTTOM = true;
        } else {
          blocks[y][x][z].faces.BOTTOM = false;
        }

        // Check Front Face (z + 1)
        if (z + 1 >= z_max || !blocks[y][x][z + 1].active) {
          blocks[y][x][z].faces.FRONT = true;
        } else {
          blocks[y][x][z].faces.FRONT = false;
        }

        // Check Back Face (z - 1)
        if (z - 1 < 0 || !blocks[y][x][z - 1].active) {
          blocks[y][x][z].faces.BACK = true;
        } else {
          blocks[y][x][z].faces.BACK = false;
        }

        // Check Right Face (x + 1)
        if (x + 1 >= x_max || !blocks[y][x + 1][z].active) {
          blocks[y][x][z].faces.RIGHT = true;
        } else {
          blocks[y][x][z].faces.RIGHT = false;
        }

        // Check Left Face (x - 1)
        if (x - 1 < 0 || !blocks[y][x - 1][z].active) {
          blocks[y][x][z].faces.LEFT = true;
        } else {
          blocks[y][x][z].faces.LEFT = false;
        }
      }
    }
  }
}
