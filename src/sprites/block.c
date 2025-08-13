#include "sprites/block.h"
#include <raylib.h>
#include <stdbool.h>

void RemoveBlock(float max_placment_distance, Player *player, Block *blocks,
                 float block_size, int y_max, int x_max, int z_max) {

  Vector3 direction = Vector3Normalize(
      Vector3Subtract(player->camera.target, player->camera.position));
  Ray ray = {player->camera.position, direction};

  RayCollision closestCollision = {0};
  closestCollision.distance = max_placment_distance;
  closestCollision.hit = false;

  int closestBlock = -1;
  for (int y = 0; y < y_max; y++) {
    for (int x = 0; x < x_max; x++) {
      for (int z = 0; z < z_max; z++) {
        int index = y * x_max * z_max + x * z_max + z;
        BoundingBox bb = {
            Vector3SubtractValue(blocks[index].position, block_size),
            Vector3AddValue(blocks[index].position, block_size)};
        RayCollision collision = GetRayCollisionBox(ray, bb);

        if (collision.hit && collision.distance < closestCollision.distance) {
          closestCollision = collision;
          closestBlock = index;
        }
      }
    }
  }

  if (closestBlock != -1) {
    blocks[closestBlock].active = false;
  }
}
void DrawBlocks(Block *blocks, float block_size, int y_max, int x_max,
                int z_max) {}

void UpdateAllBlockFaces(Block *blocks, int y_max, int x_max, int z_max) {}
