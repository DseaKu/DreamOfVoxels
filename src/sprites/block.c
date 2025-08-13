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
        // Set all blocks to active with all faces showing initially.
        // UpdateAllBlockFaces will then cull the appropriate faces.
        blocks[z][x][y].state = FACE_TOP | FACE_BOTTOM | FACE_FRONT | FACE_BACK | FACE_LEFT | FACE_RIGHT;
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
        if (blocks[z][x][y].state == 0)
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
    blocks[hit_z][hit_x][hit_y].state = 0;
  }
}
void DrawBlocks(int z_max, int x_max, int y_max,
                Block blocks[z_max][x_max][y_max], float block_size) {
  for (int z = 0; z < z_max; z++) {
    for (int x = 0; x < x_max; x++) {
      for (int y = 0; y < y_max; y++) {
        if (blocks[z][x][y].state > 0) {
          if (blocks[z][x][y].state & FACE_TOP)
            DrawCubeFace(TOP, blocks[z][x][y].position, block_size, RED);
          if (blocks[z][x][y].state & FACE_BOTTOM)
            DrawCubeFace(BOTTOM, blocks[z][x][y].position, block_size, GREEN);
          if (blocks[z][x][y].state & FACE_FRONT)
            DrawCubeFace(FRONT, blocks[z][x][y].position, block_size, BLUE);
          if (blocks[z][x][y].state & FACE_BACK)
            DrawCubeFace(BACK, blocks[z][x][y].position, block_size, YELLOW);
          if (blocks[z][x][y].state & FACE_LEFT)
            DrawCubeFace(LEFT, blocks[z][x][y].position, block_size, PURPLE);
          if (blocks[z][x][y].state & FACE_RIGHT)
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
        
        uint8_t current_state = blocks[z][x][y].state;
        if (current_state == 0) {
            // still check if it needs to become active
        } 

        uint8_t new_state = 0;

        // Check Top Face (z + 1)
        if (z + 1 >= z_max || blocks[z + 1][x][y].state == 0) {
          new_state |= FACE_TOP;
        }

        // Check Bottom Face (z - 1)
        if (z - 1 < 0 || blocks[z - 1][x][y].state == 0) {
          new_state |= FACE_BOTTOM;
        }

        // Check Front Face (y + 1)
        if (y + 1 >= y_max || blocks[z][x][y + 1].state == 0) {
          new_state |= FACE_FRONT;
        }

        // Check Back Face (y - 1)
        if (y - 1 < 0 || blocks[z][x][y - 1].state == 0) {
          new_state |= FACE_BACK;
        }

        // Check Right Face (x + 1)
        if (x + 1 >= x_max || blocks[z][x + 1][y].state == 0) {
          new_state |= FACE_RIGHT;
        }

        // Check Left Face (x - 1)
        if (x - 1 < 0 || blocks[z][x - 1][y].state == 0) {
          new_state |= FACE_LEFT;
        }
        blocks[z][x][y].state = new_state;
      }
    }
  }
}