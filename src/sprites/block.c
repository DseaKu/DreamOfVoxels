#include "sprites/block.h"
#include <raylib.h>
#include <stdbool.h>

void InitBlocks(int z_max, int x_max, int y_max,
                Block blocks[z_max][x_max][y_max], float block_size) {
  for (int z = 0; z < z_max; z++) {
    for (int x = 0; x < x_max; x++) {
      for (int y = 0; y < y_max; y++) {
        blocks[z][x][y].active = true;
        blocks[z][x][y].position =
            (Vector3){(x - x_max / 2) * block_size, z * block_size,
                      (y - y_max / 2) * block_size};
      }
    }
  }
}

void RemovePlacement(float max_placement_distance, Player *player, int z_max,
                     int x_max, int y_max,
                     Block blocks[z_max][x_max][y_max], float block_size) {

  Vector3 direction = Vector3Normalize(
      Vector3Subtract(player->camera.target, player->camera.position));
  Ray ray = {player->camera.position, direction};

  RayCollision closestCollision = {0};
  closestCollision.distance = max_placement_distance;
  closestCollision.hit = false;

  int hit_z = -1, hit_x = -1, hit_y = -1;

  for (int z = 0; z < z_max; z++) {
    for (int x = 0; x < x_max; x++) {
      for (int y = 0; y < y_max; y++) {
        if (!blocks[z][x][y].active)
          continue;

        BoundingBox bb = {
            (Vector3){blocks[z][x][y].position.x - block_size / 2,
                      blocks[z][x][y].position.y - block_size / 2,
                      blocks[z][x][y].position.z - block_size / 2},
            (Vector3){blocks[z][x][y].position.x + block_size / 2,
                      blocks[z][x][y].position.y + block_size / 2,
                      blocks[z][x][y].position.z + block_size / 2}};
        RayCollision collision = GetRayCollisionBox(ray, bb);

        if (collision.hit && collision.distance < closestCollision.distance) {
          closestCollision = collision;
          hit_z = z;
          hit_x = x;
          hit_y = y;
        }
      }
    }
  }

  if (hit_z != -1) {
    blocks[hit_z][hit_x][hit_y].active = false;
  }
}
void DrawBlocks(int z_max, int x_max, int y_max,
                Block blocks[z_max][x_max][y_max], float block_size) {
  for (int z = 0; z < z_max; z++) {
    for (int x = 0; x < x_max; x++) {
      for (int y = 0; y < y_max; y++) {
        if (blocks[z][x][y].active) {
          if (blocks[z][x][y].faces.TOP)
            DrawCube(blocks[z][x][y].position, block_size, block_size,
                     block_size, RED);
          if (blocks[z][x][y].faces.BOTTOM)
            DrawCube(blocks[z][x][y].position, block_size, block_size,
                     block_size, GREEN);
          if (blocks[z][x][y].faces.FRONT)
            DrawCube(blocks[z][x][y].position, block_size, block_size,
                     block_size, BLUE);
          if (blocks[z][x][y].faces.BACK)
            DrawCube(blocks[z][x][y].position, block_size, block_size,
                     block_size, YELLOW);
          if (blocks[z][x][y].faces.LEFT)
            DrawCube(blocks[z][x][y].position, block_size, block_size,
                     block_size, PURPLE);
          if (blocks[z][x][y].faces.RIGHT)
            DrawCube(blocks[z][x][y].position, block_size, block_size,
                     block_size, ORANGE);

          DrawCubeWires(blocks[z][x][y].position, block_size, block_size,
                        block_size, MAROON);
        }
      }
    }
  }
}

void UpdateAllBlockFaces(int z_max, int x_max, int y_max,
                         Block blocks[z_max][x_max][y_max]) {
  for (int z = 0; z < z_max; z++) {
    for (int x = 0; x < x_max; x++) {
      for (int y = 0; y < y_max; y++) {

        // If the block is not active, none of its faces should be rendered.
        if (!blocks[z][x][y].active) {
          blocks[z][x][y].faces =
              (BlockFaces){false, false, false, false, false, false};
          continue;
        }

        // Check Top Face (z + 1)
        if (z + 1 >= z_max || !blocks[z + 1][x][y].active) {
          blocks[z][x][y].faces.TOP = true;
        } else {
          blocks[z][x][y].faces.TOP = false;
        }

        // Check Bottom Face (z - 1)
        if (z - 1 < 0 || !blocks[z - 1][x][y].active) {
          blocks[z][x][y].faces.BOTTOM = true;
        } else {
          blocks[z][x][y].faces.BOTTOM = false;
        }

        // Check Front Face (y + 1)
        if (y + 1 >= y_max || !blocks[z][x][y + 1].active) {
          blocks[z][x][y].faces.FRONT = true;
        } else {
          blocks[z][x][y].faces.FRONT = false;
        }

        // Check Back Face (y - 1)
        if (y - 1 < 0 || !blocks[z][x][y - 1].active) {
          blocks[z][x][y].faces.BACK = true;
        } else {
          blocks[z][x][y].faces.BACK = false;
        }

        // Check Right Face (x + 1)
        if (x + 1 >= x_max || !blocks[z][x + 1][y].active) {
          blocks[z][x][y].faces.RIGHT = true;
        } else {
          blocks[z][x][y].faces.RIGHT = false;
        }

        // Check Left Face (x - 1)
        if (x - 1 < 0 || !blocks[z][x - 1][y].active) {
          blocks[z][x][y].faces.LEFT = true;
        } else {
          blocks[z][x][y].faces.LEFT = false;
        }
      }
    }
  }
}