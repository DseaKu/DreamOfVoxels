#include "sprites/block.h"

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
