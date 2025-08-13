#include "sprites/block.h"
#include <raylib.h>
#include <stdbool.h>

void CreatePlaneGround(Block *blocks, int *blockCount) {

  *blockCount = 0;

  // Create a ground plane
  for (int x = -MAX_LENGTH / 2; x < MAX_LENGTH / 2; x++) {
    for (int z = -MAX_LENGTH / 2; z < MAX_LENGTH / 2; z++) {
      blocks[*blockCount].position = (Vector3){(float)x, 0.0f, (float)z};
      blocks[*blockCount].active = true;
      (*blockCount)++;
    }
  }
}

void BlockPlacment(float max_placment_distance, Player *player, Block *blocks,
                   int *blockCount, float block_size) {

  Vector3 direction = Vector3Normalize(
      Vector3Subtract(player->camera.target, player->camera.position));
  Ray ray = {player->camera.position, direction};

  RayCollision closestCollision = {0};
  closestCollision.distance = max_placment_distance;
  closestCollision.hit = false;

  int closestBlock = -1;

  for (int i = 0; i < *blockCount; i++) {
    if (blocks[i].active) {
      BoundingBox bb = {Vector3SubtractValue(blocks[i].position, block_size),
                        Vector3AddValue(blocks[i].position, block_size)};
      RayCollision collision = GetRayCollisionBox(ray, bb);

      if (collision.hit && collision.distance < closestCollision.distance) {
        closestCollision = collision;
        closestBlock = i;
      }
    }
  }

  if (closestBlock != -1) {
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
      if (*blockCount < MAX_BLOCKS) {
        blocks[*blockCount].position =
            Vector3Add(blocks[closestBlock].position, closestCollision.normal);
        blocks[*blockCount].active = true;
        (*blockCount)++;
      }
    } else {
      blocks[closestBlock].active = false;
    }
  }
};
void DrawBlocks(Block *blocks, int block_count, float block_size) {

  static bool disable_faces = false;
  if (IsKeyReleased(KEY_E)) {
    disable_faces = !disable_faces;
  }

  for (int i = 0; i < block_count; i++) {
    if (blocks[i].active) {
      // Toggle faces and wire
      if (disable_faces) {
        DrawCube(blocks[i].position, block_size, block_size, block_size, GRAY);
      } else {
        DrawCubeWires(blocks[i].position, block_size, block_size, block_size,
                      MAROON);
      }
    }
  }
}

// Helper function to find if a block exists at a specific position
static bool IsBlockAt(Vector3 position, Block *allBlocks, int blockCount) {
  for (int i = 0; i < blockCount; i++) {
    if (allBlocks[i].active && allBlocks[i].position.x == position.x &&
        allBlocks[i].position.y == position.y &&
        allBlocks[i].position.z == position.z) {
      return true;
    }
  }
  return false;
}

void UpdateAllBlockFaces(Block *blocks, int blockCount) {
  for (int i = 0; i < blockCount; i++) {
    if (!blocks[i].active)
      continue;

    Vector3 current_pos = blocks[i].position;

    // Check top face
    Vector3 top_neighbor_pos = {current_pos.x, current_pos.y + 1.0f,
                                current_pos.z};
    blocks[i].faces.TOP = !IsBlockAt(top_neighbor_pos, blocks, blockCount);

    // Check bottom face
    Vector3 bottom_neighbor_pos = {current_pos.x, current_pos.y - 1.0f,
                                   current_pos.z};
    blocks[i].faces.BOTTOM = !IsBlockAt(bottom_neighbor_pos, blocks, blockCount);

    // Check front face (positive Z)
    Vector3 front_neighbor_pos = {current_pos.x, current_pos.y,
                                  current_pos.z + 1.0f};
    blocks[i].faces.FRONT = !IsBlockAt(front_neighbor_pos, blocks, blockCount);

    // Check back face (negative Z)
    Vector3 back_neighbor_pos = {current_pos.x, current_pos.y,
                                 current_pos.z - 1.0f};
    blocks[i].faces.BACK = !IsBlockAt(back_neighbor_pos, blocks, blockCount);

    // Check right face (positive X)
    Vector3 right_neighbor_pos = {current_pos.x + 1.0f, current_pos.y,
                                  current_pos.z};
    blocks[i].faces.RIGHT = !IsBlockAt(right_neighbor_pos, blocks, blockCount);

    // Check left face (negative X)
    Vector3 left_neighbor_pos = {current_pos.x - 1.0f, current_pos.y,
                                 current_pos.z};
    blocks[i].faces.LEFT = !IsBlockAt(left_neighbor_pos, blocks, blockCount);
  }
}