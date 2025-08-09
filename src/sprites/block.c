#include "sprites/block.h"

void CreatePlaneGround(Block *blocks, int *blockCount) {

  *blockCount = 0;

  // Create a ground plane
  for (int x = -8; x < 8; x++) {
    for (int z = -8; z < 8; z++) {
      blocks[*blockCount].position = (Vector3){(float)x, 0.0f, (float)z};
      blocks[*blockCount].active = true;
      (*blockCount)++;
    }
  }
}
