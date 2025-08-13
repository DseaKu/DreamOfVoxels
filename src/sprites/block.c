#include "sprites/block.h"
#include <raylib.h>
#include <stdbool.h>

void DrawCubeFace(CubeFace face, Vector3 position, float size, Color color) {
    float h = size / 2.0f;

    Vector3 v0 = {position.x - h, position.y - h, position.z + h};
    Vector3 v1 = {position.x + h, position.y - h, position.z + h};
    Vector3 v2 = {position.x + h, position.y + h, position.z + h};
    Vector3 v3 = {position.x - h, position.y + h, position.z + h};
    Vector3 v4 = {position.x - h, position.y - h, position.z - h};
    Vector3 v5 = {position.x + h, position.y - h, position.z - h};
    Vector3 v6 = {position.x + h, position.y + h, position.z - h};
    Vector3 v7 = {position.x - h, position.y + h, position.z - h};

    switch (face) {
    case FRONT:
        DrawTriangle3D(v0, v1, v2, color);
        DrawTriangle3D(v0, v2, v3, color);
        break;
    case BACK:
        DrawTriangle3D(v5, v4, v7, color);
        DrawTriangle3D(v5, v7, v6, color);
        break;
    case TOP:
        DrawTriangle3D(v3, v2, v6, color);
        DrawTriangle3D(v3, v6, v7, color);
        break;
    case BOTTOM:
        DrawTriangle3D(v4, v5, v1, color);
        DrawTriangle3D(v4, v1, v0, color);
        break;
    case RIGHT:
        DrawTriangle3D(v1, v5, v6, color);
        DrawTriangle3D(v1, v6, v2, color);
        break;
    case LEFT:
        DrawTriangle3D(v4, v0, v3, color);
        DrawTriangle3D(v4, v3, v7, color);
        break;
    }
}

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
            DrawCubeFace(TOP, blocks[z][x][y].position, block_size, RED);
          if (blocks[z][x][y].faces.BOTTOM)
            DrawCubeFace(BOTTOM, blocks[z][x][y].position, block_size, GREEN);
          if (blocks[z][x][y].faces.FRONT)
            DrawCubeFace(FRONT, blocks[z][x][y].position, block_size, BLUE);
          if (blocks[z][x][y].faces.BACK)
            DrawCubeFace(BACK, blocks[z][x][y].position, block_size, YELLOW);
          if (blocks[z][x][y].faces.LEFT)
            DrawCubeFace(LEFT, blocks[z][x][y].position, block_size, PURPLE);
          if (blocks[z][x][y].faces.RIGHT)
            DrawCubeFace(RIGHT, blocks[z][x][y].position, block_size, ORANGE);

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