#include "sprites/block.h"
#include <raylib.h>
#include <stdbool.h>

// void CreatePlaneGround(Block *blocks, int *blockCount, int y_max, int x_max)
// {
//
//   *blockCount = 0;
//
//   // Create a ground plane
//   for (int x = 0; x < x_max; x++) {
//     for (int z = 0; z < y_max; z++) {
//       blocks[*blockCount].position = (Vector3){(float)x, 0.0f, (float)z};
//       blocks[*blockCount].active = true;
//       (*blockCount)++;
//     }
//   }
// }

void BlockPlacment(float max_placment_distance, Player *player, Block *blocks,
                   int *blockCount, float block_size, int y_max, int x_max,
                   int z_max) {

  Vector3 direction = Vector3Normalize(
      Vector3Subtract(player->camera.target, player->camera.position));
  Ray ray = {player->camera.position, direction};

  RayCollision closestCollision = {0};
  closestCollision.distance = max_placment_distance;
  closestCollision.hit = false;

  for (int y = 0; y < y_max; y++) {
    for (int x = 0; x < x_max; x++) {
      for (int z = 0; z < z_max; z++) {
        int index = y * x_max * z_max + x * z_max + z;
      }
    }
  }
  // // The function accepts a simple pointer to int.
  // void process_flat(int *arr, int d1, int d2, int d3) {
  //     printf("Processing a 'flattened' array:\n");
  //     for (int i = 0; i < d1; i++) {
  //         for (int j = 0; j < d2; j++) {
  //             for (int k = 0; k < d3; k++) {
  //                 // Manual index calculation is required.
  //                 int index = i * d2 * d3 + j * d3 + k;
  //                 printf("%d ", arr[index]);
  //             }
  //             printf("\n");
  //         }
  //         printf("\n");
  //     }
  // }
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
      Vector3 pos = blocks[i].position;
      float s = block_size / 2.0f;

      // Calculate vertices relative to block position
      Vector3 v[8];
      v[0] = (Vector3){pos.x - s, pos.y - s, pos.z + s};
      v[1] = (Vector3){pos.x + s, pos.y - s, pos.z + s};
      v[2] = (Vector3){pos.x + s, pos.y + s, pos.z + s};
      v[3] = (Vector3){pos.x - s, pos.y + s, pos.z + s};
      v[4] = (Vector3){pos.x - s, pos.y - s, pos.z - s};
      v[5] = (Vector3){pos.x + s, pos.y - s, pos.z - s};
      v[6] = (Vector3){pos.x + s, pos.y + s, pos.z - s};
      v[7] = (Vector3){pos.x - s, pos.y + s, pos.z - s};

      if (disable_faces) { // Draw solid faces
        if (blocks[i].faces.FRONT) {
          DrawTriangle3D(v[0], v[1], v[2], GRAY);
          DrawTriangle3D(v[0], v[2], v[3], GRAY);
        }
        if (blocks[i].faces.BACK) {
          DrawTriangle3D(v[5], v[4], v[7], GRAY);
          DrawTriangle3D(v[5], v[7], v[6], GRAY);
        }
        if (blocks[i].faces.TOP) {
          DrawTriangle3D(v[3], v[2], v[6], GRAY);
          DrawTriangle3D(v[3], v[6], v[7], GRAY);
        }
        if (blocks[i].faces.BOTTOM) {
          DrawTriangle3D(v[4], v[5], v[1], GRAY);
          DrawTriangle3D(v[4], v[1], v[0], GRAY);
        }
        if (blocks[i].faces.RIGHT) {
          DrawTriangle3D(v[1], v[5], v[6], GRAY);
          DrawTriangle3D(v[1], v[6], v[2], GRAY);
        }
        if (blocks[i].faces.LEFT) {
          DrawTriangle3D(v[4], v[0], v[3], GRAY);
          DrawTriangle3D(v[4], v[3], v[7], GRAY);
        }
      } else { // Draw wireframe of faces
        if (blocks[i].faces.FRONT) {
          DrawLine3D(v[0], v[1], MAROON);
          DrawLine3D(v[1], v[2], MAROON);
          DrawLine3D(v[2], v[3], MAROON);
          DrawLine3D(v[3], v[0], MAROON);
        }
        if (blocks[i].faces.BACK) {
          DrawLine3D(v[5], v[4], MAROON);
          DrawLine3D(v[4], v[7], MAROON);
          DrawLine3D(v[7], v[6], MAROON);
          DrawLine3D(v[6], v[5], MAROON);
        }
        if (blocks[i].faces.TOP) {
          DrawLine3D(v[3], v[2], MAROON);
          DrawLine3D(v[2], v[6], MAROON);
          DrawLine3D(v[6], v[7], MAROON);
          DrawLine3D(v[7], v[3], MAROON);
        }
        if (blocks[i].faces.BOTTOM) {
          DrawLine3D(v[4], v[5], MAROON);
          DrawLine3D(v[5], v[1], MAROON);
          DrawLine3D(v[1], v[0], MAROON);
          DrawLine3D(v[0], v[4], MAROON);
        }
        if (blocks[i].faces.RIGHT) {
          DrawLine3D(v[1], v[5], MAROON);
          DrawLine3D(v[5], v[6], MAROON);
          DrawLine3D(v[6], v[2], MAROON);
          DrawLine3D(v[2], v[1], MAROON);
        }
        if (blocks[i].faces.LEFT) {
          DrawLine3D(v[4], v[0], MAROON);
          DrawLine3D(v[0], v[3], MAROON);
          DrawLine3D(v[3], v[7], MAROON);
          DrawLine3D(v[7], v[4], MAROON);
        }
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
    blocks[i].faces.BOTTOM =
        !IsBlockAt(bottom_neighbor_pos, blocks, blockCount);

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
